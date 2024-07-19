/*!
* Author:  Jason Hughes
* Date:    July 2024
* About:   Cost Map Logic
*
* Package: Waz
*/

#include "waz/cost_map.hpp"

CostMap::CostMap(std::string path) : params_(path)
{
    params_.setParams();
}

cv::Mat CostMap::kernalizeMask(const cv::Mat& mask, Model model) noexcept
{
    // can this be parallelized using TBB???
    cv::Mat map(params_.height, params_.width, CV_8UC1);
    for(int i=0, i_map=0; i < (mask.rows - params_.kernel) ; i += params_.kernel, ++i_map)
    {
        for (int j=0, j_map=0; j < (mask.cols - params_.kernel) ; j += params_.kernel, ++j_map)
        {
            uint8_t val;
            cv::Mat submask = mask(cv::Rect(j, i, params_.kernel, params_.kernel));
            if (model == Model::SEMANTICS)
            {
                val = mode_(submask);
            }
            else if (model == Model::DEPTH)
            {
                val = max_(submask);
            }
            map.at<uchar>(i_map, j_map) = val;
        }
    }

    return map;
}

cv::Mat CostMap::getCostMap(cv::Mat& depth, cv::Mat& semantics) noexcept
{
    cv::Mat depth_down_res = kernalizeMask(depth, Model::DEPTH);
    cv::Mat semantics_down_res = kernalizeMask(semantics, Model::SEMANTICS);
    
    // initialize the cost map as the semantic map;
    cv::Mat cost_map = semantics_down_res.clone();
   
    // must initialize cost_map from semantics before depth
    costFromSemantics(semantics_down_res, cost_map);
    costFromDepth(depth_down_res, cost_map);

    return cost_map;
}

cv::Point CostMap::getStart()
{
    return cv::Point(params_.width/2, params_.height);
}

void CostMap::costFromSemantics(cv::Mat& semantics, cv::Mat& cost_map)
{
    SemanticCost semantic_cost(cost_map, params_.label_map);
    tbb::parallel_reduce(tbb::blocked_range2d<int>(0, semantics.rows, 0, semantics.cols), semantic_cost);
}

void CostMap::costFromDepth(cv::Mat& depth, cv::Mat& cost_map)
{
    // get the pixel where the robot is
    int middle[2] = {params_.width / 2, params_.height};

    cv::Mat rect = depth(cv::Rect(middle[0] - 10, middle[1] - 20, 20, 20));
    double avg = average_(rect);

    DepthCost depth_cost(cost_map, depth, avg);
    tbb::parallel_reduce(tbb::blocked_range2d<int>(0, depth.rows, 0, depth.cols), depth_cost); 
}

// Depth Cost Calculation
CostMap::DepthCost::DepthCost(cv::Mat& cm, cv::Mat& d, double a) : cost_map(cm), depth(d), avg(a) { }

CostMap::DepthCost::DepthCost(DepthCost& dc, tbb::split) : cost_map(dc.cost_map), depth(dc.depth), avg(dc.avg) { }

void CostMap::DepthCost::operator()(const tbb::blocked_range2d<int>& r)
{ 
    for (int i = r.rows().begin(); i != r.rows().end(); ++i)
    {
        for (int j = r.cols().begin(); j != r.cols().end(); ++j)
        {
            uint8_t cost_map_val = cost_map.at<uint8_t>(i,j);
            uint8_t depth_val = depth.at<uint8_t>(i,j);

            if (cost_map_val != 255)
            {
                if ((depth_val > (avg + 10)) || (depth_val < (avg - 10)))
                {
                    cost_map.at<uint8_t>(i,j) = 255;
                }
            }
        }
    }
}

void CostMap::DepthCost::join(const DepthCost& other) { }

// Semantic Cost Calculation
CostMap::SemanticCost::SemanticCost(cv::Mat& m, std::map<int, LabelMap>& lm) : mat(m), label_map(lm) { }

CostMap::SemanticCost::SemanticCost(SemanticCost& s, tbb::split) : mat(s.mat), label_map(s.label_map) { }

void CostMap::SemanticCost::operator()(const tbb::blocked_range2d<int>& r)
{
    for (int i = r.rows().begin(); i != r.rows().end(); ++i)
    {
        for (int j = r.cols().begin(); j != r.cols().end(); ++j)
        {
            uint8_t label = mat.at<uint8_t>(i,j);
            //std::cout << "label: " << std::to_string(label) << std::endl;
            LabelMap ll = label_map[label];
            mat.at<uint8_t>(i, j) = ll.cost;
        }
    }
} 

void CostMap::SemanticCost::join(const SemanticCost& other) { }


void CostMap::CostMapParams::setParams() noexcept
{
    width = params_map_["sizing"]["width"].asInt();
    height = params_map_["sizing"]["height"].asInt();
    kernel = params_map_["sizing"]["kernel"].asInt();

    int num_classes = params_map_["sizing"]["num_classes"].asInt();

    for (uint8_t i=0; i < num_classes; ++i)
    {
        std::string i_str = std::to_string(i);
        std::string label = params_map_["classes"][i_str]["id"].asString();
        uint8_t cost = static_cast<uint8_t>(params_map_["classes"][i_str]["cost"].asInt());
        label_map[i] = LabelMap(label, cost, i);
    }
}


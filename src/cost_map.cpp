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
    cv::Mat depth_down_res;
    cv::Mat semantics_down_res;

    if (params_.kernel > 1)
    {
        depth_down_res = kernalizeMask(depth, Model::DEPTH);
        semantics_down_res = kernalizeMask(semantics, Model::SEMANTICS);
    }
    else
    {
        semantics_down_res = semantics;
        depth_down_res = depth;
    }

    // initialize the cost map as the semantic map;
    cv::Mat cost_map = semantics_down_res.clone();

    int middle[2] = {params_.width / 2, params_.height};

    cv::Mat rect = depth_down_res(cv::Rect(middle[0] - 10, middle[1] - 20, 20, 20));
    double avg = average_(rect);
    

    BuildCostMap builder(depth_down_res, semantics_down_res, cost_map, params_.label_map, avg);
    tbb::parallel_reduce(tbb::blocked_range2d<int>(0, cost_map.rows, 0, cost_map.cols), builder);

    cost_map.row(params_.height-1).setTo(cv::Scalar(0));

    //BufferCostMap bufferer(cost_map, params_.buffer);
    //tbb::parallel_reduce(tbb::blocked_range2d<int>(0, cost_map.rows, 0, cost_map.cols), bufferer);
    cost_map = fillCostMap(cost_map);

    return cost_map;
}

cv::Point CostMap::getStart()
{
    return cv::Point(params_.width/2, params_.height);
}

int CostMap::getScale() const noexcept
{   
    return params_.kernel;
}


// Build Cost Map, a combination of SemanticCost and DepthCost
CostMap::BuildCostMap::BuildCostMap(cv::Mat& d, cv::Mat& s, cv::Mat& cm, std::map<int,LabelMap>& lm, double a) : depth(d), semantics(s), cost_map(cm), label_map(lm), avg(a) { }

CostMap::BuildCostMap::BuildCostMap(BuildCostMap& bcm, tbb::split) : depth(bcm.depth), semantics(bcm.semantics), cost_map(bcm.cost_map), label_map(bcm.label_map), avg(bcm.avg) { }

void CostMap::BuildCostMap::operator()(const tbb::blocked_range2d<int>& r)
{ 
    for (int i = r.rows().begin(); i != r.rows().end(); ++i)
    {
        for (int j = r.cols().begin(); j != r.cols().end(); ++j)
        {
            uint8_t label = semantics.at<uint8_t>(i,j);
            LabelMap ll = label_map[label];
            cost_map.at<uint8_t>(i, j) = ll.cost;

            uint8_t depth_val = depth.at<uint8_t>(i,j);

            if (ll.cost != 255)
            {
                if ((depth_val > (avg + 20)) || (depth_val < (avg - 20)))
                {
                    cost_map.at<uint8_t>(i,j) = 255;
                }
            }
            
            

        }
    }
}

void CostMap::BuildCostMap::join(const BuildCostMap& other) { }

cv::Mat CostMap::fillCostMap(cv::Mat& cost_map)
{
    int buff2 = params_.buffer / 2;
    int buff = params_.buffer;
    cv::Mat cm_clone = cost_map.clone();

    for (int i=0; i < cost_map.rows; ++i)
    {
        for (int j=0; j<cost_map.cols; ++j)
        { 
            if (i-buff2>0 && i < cost_map.cols-buff2 && j-buff2>0 && j < cost_map.rows-buff2 && cost_map.at<uint8_t>(i,j) == 255)
            {
                cv::Mat kern = cm_clone(cv::Rect(i-buff2, j-buff2, buff, buff));
                for (int k=0; k < kern.rows; ++k)
                {
                    for (int l=0; l < kern.cols; ++l)
                    {
                        if (kern.at<uint8_t>(k,l) != 255)
                        {
                            kern.at<uint8_t>(k,l) = 128;
                        }
                    }
                }
            }
        }
    }
    return cm_clone;
}
    
// add a buffer around objects in the cost map
CostMap::BufferCostMap::BufferCostMap(cv::Mat& cm, int b) : cost_map(cm), buff(b) { }

CostMap::BufferCostMap::BufferCostMap(BufferCostMap& bcm, tbb::split) : cost_map(bcm.cost_map), buff(bcm.buff) { }

void CostMap::BufferCostMap::operator()(const tbb::blocked_range2d<int>& r)
{
    for (int i = r.rows().begin(); i != r.rows().end(); ++i)
    {
        for (int j = r.cols().begin(); j != r.cols().end(); ++j)
        {
            int buff2 = buff / 2;
            if (i-buff2>0 && i < cost_map.cols-buff2 && j-buff2>0 && j < cost_map.rows-buff2 && cost_map.at<uint8_t>(i,j) == 255)
            {
                cv::Mat kern = cost_map(cv::Rect(i-buff2, j-buff2, buff, buff));
                for (int k=0; k < kern.rows; ++k)
                {
                    for (int l=0; l < kern.cols; ++l)
                    {
                        if (kern.at<uint8_t>(k,l) != 255)
                        {
                            kern.at<uint8_t>(k,l) = 128;
                        }
                    }
                }
            }
        }
    }
}

void CostMap::BufferCostMap::buffer(cv::Mat& cost_map, uint8_t dir, int i, int j)
{
    if (dir == 0)
    {
        cost_map(cv::Range(i-buff,i), cv::Range(j,j+1)) = 128;
    }
    else if (dir == 1)
    {
        cost_map(cv::Range(i,i+buff), cv::Range(j,j+1)) = 128;
    }
    else if (dir == 2)
    {
        cost_map(cv::Range(i,i+1), cv::Range(j-buff,j)) = 128;
    }
    else if (dir == 3)
    {
        cost_map(cv::Range(i,i+1), cv::Range(j,j+buff)) = 128;
    }
}

void CostMap::BufferCostMap::join(const BufferCostMap& other) { }

void CostMap::CostMapParams::setParams() noexcept
{
    width = params_map_["sizing"]["width"].asInt();
    height = params_map_["sizing"]["height"].asInt();
    kernel = params_map_["sizing"]["kernel"].asInt();
    buffer = params_map_["sizing"]["buffer"].asInt();
    threshold = params_map_["sizing"]["threshold"].asInt();

    int num_classes = params_map_["sizing"]["num_classes"].asInt();

    for (uint8_t i=0; i < num_classes; ++i)
    {
        std::string i_str = std::to_string(i);
        std::string label = params_map_["classes"][i_str]["id"].asString();
        uint8_t cost = static_cast<uint8_t>(params_map_["classes"][i_str]["cost"].asInt());
        label_map[i] = LabelMap(label, cost, i);
    }
}


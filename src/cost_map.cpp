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

}


cv::Mat CostMap::kernalizeMask(const cv::Mat& mask, Model model) noexcept
{
    // can this be parallelized using TBB???
    cv::Mat map(params_.height, params_.width, CV_8U);

    for(int i=0, i_map=0; i < (params_.height - params_.kernel) ; i += params_.kernel, ++i_map)
    {
        for (int j=0, j_map=0; j < (params_.width - params_.kernel) ; i += params_.kernel, ++j_map)
        {
            uint8_t val;
            cv::Mat submask = mask(cv::Rect(i, j, params_.kernel, params_.kernel));
            if (model == Model::SEMANTICS)
            {
                val = mode_(submask);
            }
            else if (model == Model::DEPTH)
            {
                val = max_(submask);
            }
            map.at<int>(i_map, j_map) = val;
        }
    }

    return map;
}

CostMap::Initializer::Initializer(cv::Mat& m, CostMap* cm) : mat(m), cost_map(cm) { }

CostMap::Initializer::Initializer(Initializer& s, tbb::split) : mat(s.mat) { }

void CostMap::Initializer::operator()(const tbb::blocked_range2d<int>& r)
{
    for (int i = r.rows().begin(); i != r.rows().end(); ++i)
    {
        for (int j = r.cols().begin(); j != r.cols().end(); ++j)
        {
            uint8_t label = mat.at<uint8_t>(i,j);
            LabelMap lm = cost_map->params_.label_map[label];
            mat.at<uint8_t>(i, j) = lm.cost;
        }
    }
}   

void CostMap::CostMapParams::setParams() noexcept
{
    width = params_map_["width"].asInt();
    height = params_map_["height"].asInt();
    kernel = params_map_["kernel"].asInt();

    int num_classes = params_map_["num_classes"].asInt();

    for (uint8_t i=0; i < num_classes; ++i)
    {
        std::string i_str = std::to_string(i);
        std::string label = params_map_["classes"][i_str]["id"].asString();
        uint8_t cost = params_map_["classes"][i_str]["cost"].asInt();

        label_map[i] = LabelMap(label, cost, i);
    }
}

LabelMap CostMap::getLabelMap(uint8_t label)
{
    return params_.label_map[label];
}   

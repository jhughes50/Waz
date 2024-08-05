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
    // Not in a thread safe way
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

    pre_filled_ = cost_map.clone();

    //cv::Mat for_filling = cost_map(cv::Range(params_.pixel_width, cost_map.rows-1), cv::Range::all());
    BufferRow buffer_row(cost_map, params_.pixel_width, params_.edge_buffer);
    tbb::parallel_reduce(tbb::blocked_range<int>(0, cost_map.rows), buffer_row);

    BufferCol buffer_col(cost_map, params_.vertical_buffer, params_.edge_buffer);
    tbb::parallel_reduce(tbb::blocked_range<int>(0, cost_map.cols), buffer_col);
    //fillCostMap(cost_map);

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

cv::Mat CostMap::getPreFilled() const noexcept
{
    return pre_filled_;
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

void CostMap::fillCostMap(cv::Mat& cost_map)
{
    int buff2 = params_.buffer / 2;
    int buff = params_.buffer;
    //cv::Mat cm_clone = cost_map.clone();

    for (int i=0; i < cost_map.rows; ++i)
    { 
        cv::Mat cm_row = cost_map.row(i);
        for (int j = 1; j < cost_map.cols-1; ++j)
        {
            uint8_t val = cost_map.at<uint8_t>(i,j);
            uint8_t val_m1 = cost_map.at<uint8_t>(i,j-1);
            uint8_t val_p1 = cost_map.at<uint8_t>(i,j+1);

            if (val == 0 && val_p1 == 255) // backfill
            {
                if (j <= buff)
                {
                    cm_row(cv::Range::all(), cv::Range(0,j)).setTo(255);
                }
                else
                {
                    cm_row(cv::Range::all(), cv::Range(j-buff, j+1)).setTo(255);
                }
            }
            else if (val == 255 && val_p1 == 0) // forwardfill
            {
                if (j > cost_map.cols-buff)
                {
                    cm_row(cv::Range::all(),cv::Range(j,cost_map.cols-1)).setTo(255);
                }
                else
                {
                    cm_row(cv::Range::all(), cv::Range(j, j+buff)).setTo(255);
                }
            }
        }
    }
    //return cm_clone;
}
    
// add a buffer around objects in the cost map
// Buffer the width
CostMap::BufferRow::BufferRow(cv::Mat& cm, int pw, int b) : cost_map(cm), pixel_width(pw), buffer(b) { }

CostMap::BufferRow::BufferRow(BufferRow& bcm, tbb::split) : cost_map(bcm.cost_map), pixel_width(bcm.pixel_width), buffer(bcm.buffer) { }

void CostMap::BufferRow::operator()(const tbb::blocked_range<int>& range)
{
    
    for (int row = range.begin(); row < range.end(); ++row)
    {
        cv::Mat cm_row = cost_map.row(row);
        int width = 0;
        if (cost_map.rows - row <= pixel_width)
        {
            width = pixel_width - (cost_map.rows - row);
        }
        else continue;

        for (int i = 1; i < cost_map.cols-buffer; ++i)
        {
            uint8_t val = cost_map.at<uint8_t>(row,i);
            uint8_t val_m1 = cost_map.at<uint8_t>(row,i-1);
            uint8_t val_p1 = cost_map.at<uint8_t>(row,i+1);

            if (val == 0 && val_p1 == 255) // backfill
            {
                if (i <= width)
                {
                    cm_row(cv::Range::all(), cv::Range(0, i)).setTo(255);
                }
                else
                {
                    cm_row(cv::Range::all(), cv::Range(i-width, i+1)).setTo(255);
                }
            }
            else if (val == 255 && val_p1 == 0) // forwardfill
            {
                if (i > cost_map.cols-width)
                {
                    cm_row(cv::Range::all(), cv::Range(i, cost_map.cols-1)).setTo(255);
                }
                else
                {
                    cm_row(cv::Range::all(), cv::Range(i, i+width)).setTo(255);
                    i+= width+1;
                }
            }
        }
    }
}

void CostMap::BufferRow::join(const BufferRow& other) { }

// buffer the column
CostMap::BufferCol::BufferCol(cv::Mat& cm, int vb, int b) : cost_map(cm), vertical_buffer(vb), buffer(b) { }

CostMap::BufferCol::BufferCol(BufferCol& bcm, tbb::split) : cost_map(bcm.cost_map), vertical_buffer(bcm.vertical_buffer), buffer(bcm.buffer) { }

void CostMap::BufferCol::operator()(const tbb::blocked_range<int>& range)
{
    for (int col = range.begin(); col < range.end(); ++col)
    {
        cv::Mat cm_col = cost_map.col(col);
        for (int i = 1; i < cost_map.rows-buffer; ++i)
        { 
            uint8_t val = cost_map.at<uint8_t>(i,col);
            uint8_t val_m1 = cost_map.at<uint8_t>(i-1,col);
            uint8_t val_p1 = cost_map.at<uint8_t>(i+1,col);

            if (val == 255 && val_p1 == 0) // upfill
            {
                if (i <= vertical_buffer)
                {
                    cm_col(cv::Range(0, i), cv::Range::all()).setTo(255);
                }
                else
                {
                    cm_col(cv::Range(i-vertical_buffer, i+1), cv::Range::all()).setTo(255);
                }
            }
            else if (val == 255 && val_p1 == 0) // downfill
            {
                if (i > cost_map.cols-vertical_buffer)
                {
                    cm_col(cv::Range(i, cost_map.cols-1), cv::Range::all()).setTo(255);
                }
                else
                {
                    cm_col(cv::Range(i, i+vertical_buffer), cv::Range::all()).setTo(255);
                    i+= vertical_buffer+1;
                }
            }
        }
    }
}

void CostMap::BufferCol::join(const BufferCol& other) { }

void CostMap::CostMapParams::setParams() noexcept
{
    width = params_map_["sizing"]["width"].asInt();
    height = params_map_["sizing"]["height"].asInt();
    kernel = params_map_["sizing"]["kernel"].asInt();
    buffer = params_map_["sizing"]["buffer"].asInt();
    threshold = params_map_["sizing"]["threshold"].asInt();
    edge_buffer = params_map_["sizing"]["edge_buffer"].asInt();
    vertical_buffer = params_map_["sizing"]["vertical_buffer"].asInt();

    pixel_width = params_map_["robot"]["pixel_width"].asInt() / kernel;

    int num_classes = params_map_["sizing"]["num_classes"].asInt();
    
    for (uint8_t i=0; i < num_classes; ++i)
    {
        std::string i_str = std::to_string(i);
        std::string label = params_map_["classes"][i_str]["id"].asString();
        uint8_t cost = static_cast<uint8_t>(params_map_["classes"][i_str]["cost"].asInt());
        label_map[i] = LabelMap(label, cost, i);
    }
}


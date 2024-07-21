/*!
* Author:  Jason Hughes
* Date:    July 2024
* About:   Orchestrator
*
* Package: Waz
*/

#include "waz/waz.hpp"

Waz::Waz(std::string path) : depth_(path+"networks.json"), semantics_(path+"networks.json"), cost_map_(path+"cost_map.json") { }

Waz::~Waz()
{
    //delete astar_;
}

std::vector<cv::Point> Waz::operator()(cv::Mat& img, cv::Point& goal)
{
    cv::Mat depth = depth_.inference(img);
    cv::Mat semantics = semantics_.inference(img);

    current_cost_map_ = cost_map_.getCostMap(depth, semantics);
    cv::Point start = cost_map_.getStart();
    
    assert(current_cost_map_.at<uchar>(start) != 255);

    std::vector<cv::Point> path = astar_.search(current_cost_map_, start, goal);

    return path;
}

cv::Mat Waz::drawPath(cv::Mat img, std::vector<cv::Point> path)
{
    int scale = 8;
    UpscalePath up(path, scale);

    tbb::parallel_reduce(tbb::blocked_range<int>(0,path.size()), up);

    for(const auto& point : path)
    {
        cv::circle(img, point, 1, cv::Scalar(0,0,255), -1);    
    }

    return img;
}

cv::Mat Waz::getCostMap() const noexcept
{
    return current_cost_map_;
}

std::vector<cv::Point> Waz::getPath() const noexcept
{
    return current_path_;
}

Waz::UpscalePath::UpscalePath(std::vector<cv::Point>& p, int& s) : path(p), scale(s) { }

Waz::UpscalePath::UpscalePath(UpscalePath& up, tbb::split) : path(up.path), scale(up.scale) { }

void Waz::UpscalePath::operator()(const tbb::blocked_range<int>& r)
{
    for (int i = r.begin(); i != r.end(); ++i)
    {
        path[i].x = path[i].x * scale;
        path[i].y = path[i].y * scale;
    }
}

void Waz::UpscalePath::join(const UpscalePath& other) { }

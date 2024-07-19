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

    cv::Mat cost_map = cost_map_.getCostMap(depth, semantics);
    cv::Point start = cost_map_.getStart();

    std::vector<cv::Point> path = astar_.search(cost_map, start, goal);

    return path;
}

cv::Mat Waz::drawPath(cv::Mat img, std::vector<cv::Point> path)
{
    for(const auto& point : path)
    {
        cv::circle(img, point, 3, cv::Scalar(255,0,0), -1);    
    }

    return img;
}

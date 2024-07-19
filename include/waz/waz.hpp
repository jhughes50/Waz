/*!
* Author:  Jason Hughes
* Date:    July 2024
* About:   Orchestrator for the waz modules.
*          This will take in an image and pass it 
*          to the model managers, take the output from the 
*          two models and get a cost map. It will then search the 
*          cost map and return a path.
*
* Package: Waz
*/

#ifndef WAZ_HPP
#define WAZ_HPP

#include "depth.hpp"
#include "semantics.hpp"
#include "cost_map.hpp"
#include "search.hpp"

class Waz
{
    public:
        Waz() = default;
        Waz(std::string path);
        ~Waz();
        
        std::vector<cv::Point> operator()(cv::Mat& img, cv::Point& goal);
        cv::Mat drawPath(cv::Mat img, std::vector<cv::Point> path);

    private:

        DepthManager depth_;
        SemanticsManager semantics_;
        CostMap cost_map_;
        AStarSearch astar_;

};
#endif 

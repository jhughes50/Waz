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

#include <opencv2/opencv.hpp>
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>
#include <tbb/parallel_reduce.h>

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
    
        cv::Mat getCostMap() const noexcept;
        std::vector<cv::Point> getPath() const noexcept;

        struct UpscalePath
        {
            std::vector<cv::Point>& path;
            int& scale;

            UpscalePath(std::vector<cv::Point>& p, int& s);
            UpscalePath(UpscalePath& up, tbb::split);

            void operator()(const tbb::blocked_range<int>& r);
            void join(const UpscalePath& other); 
        };

    private:

        DepthManager depth_;
        SemanticsManager semantics_;
        CostMap cost_map_;
        AStarSearch astar_;

        cv::Mat current_cost_map_;
        std::vector<cv::Point> current_path_;

};
#endif 

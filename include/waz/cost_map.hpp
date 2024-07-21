/* Author:  Jason Hughes
*  Date:    June 2024
*  About:   Cost Map Object influnced by relative depth
*           and semantic segmentation
*  Package: waz
*/

#ifndef COST_MAP_HPP
#define COST_MAP_HPP

#include <string>
#include <map>
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>
#include <tbb/parallel_reduce.h>
#include <tbb/blocked_range2d.h>

#include "label_map.hpp"
#include "params.hpp"
#include "model.hpp"
#include "average.hpp"
#include "mode.hpp"
#include "max.hpp"

class CostMap
{
    public:
        //CostMap() = default;
        //CostMap(int width, int height);
        CostMap(std::string path);

        cv::Mat kernalizeMask(const cv::Mat& mask, Model model) noexcept;
        cv::Mat getCostMap(cv::Mat& depth, cv::Mat& semantics) noexcept;
        cv::Mat getCostMap() const noexcept;
        cv::Point getStart();

        struct DepthCost
        {
            cv::Mat& cost_map;
            cv::Mat& depth;
            double avg;

            DepthCost(cv::Mat& cm, cv::Mat& d, double avg);
            DepthCost(DepthCost& dc, tbb::split);

            void operator()(const tbb::blocked_range2d<int>& r);
            void join(const DepthCost& other);
        };
        
        struct SemanticCost
        {
            cv::Mat& mat;
            std::map<int, LabelMap>& label_map;

            SemanticCost(cv::Mat& m, std::map<int,LabelMap>& lm);
            SemanticCost(SemanticCost& s, tbb::split);

            void operator()(const tbb::blocked_range2d<int>& r);
            void join(const SemanticCost& other);
        };

        struct CostMapParams : public Params 
        {
            using Params::Params;

            int width, height;
            int kernel;
            std::map<int, LabelMap> label_map;

            void setParams() noexcept;
        };

        friend struct Initializer;

    private:
        void costFromSemantics(cv::Mat& semantics, cv::Mat& cost_map);
        void costFromDepth(cv::Mat& depth, cv::Mat& cost_map); 

        CostMapParams params_;
        Average average_;
        Mode mode_;
        Max max_;
};
#endif

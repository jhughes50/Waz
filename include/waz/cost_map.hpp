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
#include "mode.hpp"
#include "max.hpp"

class CostMap
{
    public:
        //CostMap() = default;
        //CostMap(int width, int height);
        CostMap(std::string path);

        struct Initializer
        {
            cv::Mat& mat;
            CostMap* cost_map;

            Initializer(cv::Mat& m, CostMap* cm);
            Initializer(Initializer& s, tbb::split);

            void operator()(const tbb::blocked_range2d<int>& r);
        };

        struct CostMapParams : public Params 
        {
            using Params::Params;

            int width, height;
            int kernel;
            std::map<int, LabelMap> label_map;

            void setParams() noexcept;
        };

        LabelMap getLabelMap(uint8_t label);

        friend struct Initializer;

    private:
        static const int SIZE = 256;
        Eigen::Matrix<uint8_t, SIZE, SIZE> cost_map_;
    
        void searchDepthForObjects(const Eigen::MatrixXf& depth);
        cv::Mat kernalizeMask(const cv::Mat& mask, Model model) noexcept;
        
        CostMapParams params_;
        Mode mode_;
        Max max_;
};
#endif

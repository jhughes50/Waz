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
#include <tbb/parallel_for.h>

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
        int getScale() const noexcept;
        cv::Point getStart();
        cv::Mat getPreFilled() const noexcept;

        struct BufferCol
        {
            cv::Mat& cost_map;
            int vertical_buffer;
            int buffer;

            BufferCol(cv::Mat& cm, int vb, int b);
            BufferCol(BufferCol& bcm, tbb::split);

            void operator()(const tbb::blocked_range<int>& range);
            void join(const BufferCol& other);
        };

        struct BufferRow
        {
            cv::Mat& cost_map;
            int pixel_width;
            int buffer;

            BufferRow(cv::Mat& cm, int pw, int b);
            BufferRow(BufferRow& bcm, tbb::split);

            void operator()(const tbb::blocked_range<int>& range);
            void join(const BufferRow& other);
        };

        struct BuildCostMap
        {
            cv::Mat& depth;
            cv::Mat& semantics;
            cv::Mat& cost_map;

            std::map<int, LabelMap>& label_map;
            double avg;

            BuildCostMap(cv::Mat& d, cv::Mat& s, cv::Mat& cm, std::map<int,LabelMap>& lm, double a);
            BuildCostMap(BuildCostMap& bcm, tbb::split);

            void operator()(const tbb::blocked_range2d<int>& r);
            void join(const BuildCostMap& other);
        };

        struct CostMapParams : public Params 
        {
            using Params::Params;

            int width, height;
            int kernel;
            int buffer, edge_buffer, vertical_buffer;
            int threshold;
            int pixel_width;
            std::map<int, LabelMap> label_map;

            void setParams() noexcept;
        };

        friend struct Initializer;

    private:

        void fillCostMap(cv::Mat& cost_map);

        cv::Mat pre_filled_;

        CostMapParams params_;
        Average average_;
        Mode mode_;
        Max max_;
};
#endif

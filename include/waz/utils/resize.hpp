/*! 
* Author:  Jason Hughes
* Date:    July 2024
* About:   Resize method to resize the image according
*          to the patch constraint.
*
* Package: Waz
*/

#ifndef RESIZE_HPP
#define RESIZE_HPP

#include <cmath>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class Resize
{   
    public:
        Resize(int width, int height, bool resize_target = true, int multiple_of = 1);
        Resize(int width, int height);
        Resize() = default;

        void operator()(cv::Mat& img);
        void operator()(cv::Mat& img, const int width, const int height);

    private:
        int width_;
        int height_;
        bool resize_target_;
        int multiple_of_;

        int constrainToMultipleOf(const double x, const int min_val = -1, const int max_val = -1);
        cv::Size getSize(const int width, const int height);
};
#endif

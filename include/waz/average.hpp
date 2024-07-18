/*!
* Author:  Jason Hughes
* Date:    July 2024
* About:   Module to calculate average of an opencv matrix
*
* Package: Waz
*/

#ifndef AVERAGE_HPP
#define AVERAGE_HPP

#include <opencv2/opencv.hpp>

struct Average
{
    Average() = default;
    double operator()(cv::Mat& mat) noexcept;
};
#endif

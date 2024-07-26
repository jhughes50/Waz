/*!
* Author: Jason Hughes
* Date:   July 2024
* About:  Find the Max value of 
*         an opencv matrix.
*
* Package: Waz
*/

#ifndef MAX_HPP
#define MAX_HPP

#include <opencv2/opencv.hpp>

struct Max
{
    Max() = default;

    uint8_t operator()(cv::Mat& mat) noexcept;
};
#endif

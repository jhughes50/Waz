/*!
* Author:  Jason Hughes
* Date:    July 2024
* About:   A quick object to calculate mode
*          from a cv::Mat, typed.
*
* Package: Waz
*/

#ifndef MODE_HPP
#define MODE_HPP

#include <opencv2/opencv.hpp>
#include <unordered_map>

struct Mode
{
    Mode() = default;

    uint8_t operator()(cv::Mat& mat);
};
#endif

/*!
* Author:  Jason Hughes
* Date:    July 2024
* About:   Interpolate using torch
*
* Package: Waz
*/

#ifndef INTERPOLATE_HPP
#define INTERPOLATE_HPP

#include <torch/torch.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

struct Interpolate
{
    Interpolate() = default;
    at::Tensor operator()(at::Tensor& tensor, int h, int w);
    void operator()(cv::Mat& img, int h, int w);
};
#endif

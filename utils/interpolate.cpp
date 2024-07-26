/*!
* Author:  Jason Hughes
* Date:    July 2024
* About:   Interpolate using torch
*
* Package: Waz
*/

#include "waz/utils/interpolate.hpp"

at::Tensor Interpolate::operator()(at::Tensor& tensor, int h, int w)
{
    return torch::nn::functional::interpolate(
        tensor,  
        torch::nn::functional::InterpolateFuncOptions()
            .size(std::vector<int64_t>{h, w})
            .mode(torch::kBilinear)
            .align_corners(false)).squeeze(0).squeeze(0);
}

void Interpolate::operator()(cv::Mat& img, int h, int w)
{
    cv::resize(img, img, cv::Size(w,h), 0, 0, cv::INTER_LINEAR);
}

/*!
* Author:  Jason Hugehs
* Date:    July 2024
* About:   Image resizer based on depth anything resizer
*
* Package: Waz
*/

#include "waz/resize.hpp"
#include <iostream>

Resize::Resize(int width, int height, bool resize_target, int multiple_of)
{
    width_ = width;
    height_ = height;
    resize_target_ = resize_target;
    multiple_of_ = multiple_of;
}

Resize::Resize(int width, int height)
{
    width_ = width;
    height_ = height;
}

int Resize::constrainToMultipleOf(const double x, const int min_val, const int max_val)
{
    int y;
    y = static_cast<int>(std::round(x/(double)multiple_of_) * multiple_of_);

    if (max_val != -1 && y > max_val)
    {
        y = static_cast<int>(std::floor(x/(double)multiple_of_) * multiple_of_);
    }

    if (y < min_val)
    {
        y = static_cast<int>(std::ceil(x/(double)multiple_of_) * multiple_of_);
    }

    return y;
}

cv::Size Resize::getSize(const int width, const int height)
{
    double scale_height = height_ /(double) height;
    double scale_width = width_ /(double) width;
    // using lower_bound resize method
    (scale_width > scale_height) ? scale_height = scale_width : scale_width = scale_height;

    int new_height, new_width;
    new_height = constrainToMultipleOf(scale_height * height, height_);
    new_width  = constrainToMultipleOf(scale_width * width, width_);
    cv::Size dims(new_width, new_height);

    return dims;
}

void Resize::operator()(cv::Mat& img)
{
    cv::Size dims = getSize(img.cols, img.rows);
    
    cv::resize(img, img, dims, cv::INTER_AREA);
}

void Resize::operator()(cv::Mat& img, const int width, const int height)
{
    cv::Size dims(width, height);

    cv::resize(img, img, dims, cv::INTER_AREA);
}

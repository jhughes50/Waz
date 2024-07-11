/*!
* Author:  Jason Hughes
* Date:    July 2024
* About:   Normalizes an image according to
*          given mean and standard deviation
*
* Packages: Waz
*/

#include "waz/normalize.hpp"

Normalize::Normalize(const int mean[SIZE], const int std[SIZE]) : mean_(mean[0], mean[1], mean[2]), std_(std[0], std[1], std[2]) { }

void Normalize::operator()(cv::Mat& img)
{ 
    // convert from BGR to RGB
    cv::cvtColor(img, img, cv::COLOR_RGB2BGR);
    img.convertTo(img, 5, 1.0/255.0);

    // divide by mean subtract std
    cv::subtract(img, mean_, img);
    cv::divide(img, std_, img);
}

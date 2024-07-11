/*! 
* Author:  Jason Hughes
* Date:    July 2024
* About:   Normalize the image according to 
*          to a given mean and standard deveiation
*          for each channel.
*
* Package: Waz
*/

#ifndef NORMALIZE_HPP
#define NORMALIZE_HPP

#include <cmath>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class Normalize
{

    private:
        static const int SIZE = 3;
        cv::Scalar mean_;
        cv::Scalar std_;
    
    public:
        Normalize(const float mean[SIZE], const float std[SIZE]);
        Normalize() = default;

        void operator()(cv::Mat& img);
        void operator()(cv::Mat& img, const float mean[SIZE], const float std[SIZE]);
};
#endif

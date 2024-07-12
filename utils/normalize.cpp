/*!
* Author:  Jason Hughes
* Date:    July 2024
* About:   Normalizes an image according to
*          given mean and standard deviation
*
* Package: Waz
*/

#include "waz/normalize.hpp"

Normalize::Normalize(const float mean[SIZE], const float std[SIZE]) : mean_(mean[0], mean[1], mean[2]), std_(std[0], std[1], std[2]) { }

void Normalize::operator()(cv::Mat& img, const float mean[SIZE], const float std[SIZE], bool norm)
{ 
    // convert from BGR to RGB
    cv::cvtColor(img, img, cv::COLOR_RGB2BGR);
    if (norm)
    {
        img.convertTo(img, CV_32F, 1.0/255.0);
    }
    else
    {
        img.convertTo(img, CV_32F);
    }
    // divide by mean subtract std 
    std::vector<cv::Mat> channels;
    cv::split(img, channels);

    for (int i = 0; i < channels.size(); ++i)
    {
        channels[i] = (channels[i] - mean[i]) - std[i];
    }

    cv::merge(channels, img);
}

void Normalize::operator()(cv::Mat& img, const float mean[SIZE], const float std[SIZE])
{
    img.convertTo(img, CV_32F);

    std::vector<cv::Mat> channels;
    cv::split(img, channels);

    for (int i = 0; i < channels.size(); ++i)
    {
        channels[i] = (channels[i] - mean[i]) - std[i];
    }

    cv::merge(channels, img);
    cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
}

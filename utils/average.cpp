/*! 
* Author:  Jason Hughes
* Date:    July 2024
* About:   Module to calculate the average
*          of an opencv matrix.
*
* Package: Waz
*/

#include <waz/average.hpp>

double Average::operator()(cv::Mat& mat) noexcept 
{
    int sum = 0;
    int count = 0;

    for (int i = 0; i < mat.rows; ++i)
    {
        for (int j = 0; j < mat.cols; ++j)
        {
            sum += mat.at<uchar>(i,j);
            count ++;
        }
    }
    
    return sum /(double) count;
}

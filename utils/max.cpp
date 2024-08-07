/*!
* Author:  Jason Hughes
* Date:    July 2024
* About:   find the max value of a 
*          cv Matrix.
*
* Package: Waz
*/

#include "waz/utils/max.hpp"

uint8_t Max::operator()(cv::Mat& mat) noexcept
{
    uint8_t max;
    max = mat.at<uint8_t>(0,0);

    for (int i = 0; i < mat.rows; i++)
    {
        for (int j = 0; j < mat.cols; j++)
        {
            if (mat.at<uint8_t>(i,j) > max)
            {
                max = mat.at<uint8_t>(i,j);
            }
        }
    }

    return max;
}

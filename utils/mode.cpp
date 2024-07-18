/*!
* Author:  Jason Hughes
* Date:    July 2024
* About:   calculate the mode of a 
*          cv matrix
*
* Package: Waz 
*/

#include "waz/mode.hpp"

uint8_t Mode::operator()(cv::Mat& mat)
{
    std::unordered_map<uint8_t, int> frequency;
    for (int i = 0; i < mat.rows; ++i) 
    {
        for (int j = 0; j < mat.cols; ++j)
        {
            uint8_t value = mat.at<int>(i, j);  
            frequency[value]++;
        }
    }
    uint8_t mode = mat.at<uint8_t>(0, 0);  
    int max_count = frequency[mode];
    
    for (const auto& kv : frequency) 
    {
        if (kv.second > max_count) 
        {
            mode = kv.first;
            max_count = kv.second;
        }
    }

    return mode;
}

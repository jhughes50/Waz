/*!
* Author:  Jason Hughes
* Date:    July 2024
* About:   Test image normalization and resizing
*
* Package: Waz
*/

#include "waz/utils/resize.hpp"
#include "waz/utils/normalize.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp> 
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

int main(int argc, char **argv)
{

    cv::Mat test_img = cv::imread("/home/jason/test/imgs/test-img-1.png", cv::IMREAD_COLOR);
    if (test_img.empty())
    {
        std::cout << "[TEST] Cound not find test image" << std::endl;
        return -1;
    }
    else
    {
        std::cout << "[TEST] Loaded test image" << std::endl;
    }
    
    Normalize normalize;
    Resize resize;

    float mean[3] = {123.675, 116.28, 103.53};
    float std[3]  = {58.395, 57.12, 57.365};

    normalize(test_img, mean, std);
    resize(test_img, 1024, 1024);

    cv::imshow("test", test_img);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}

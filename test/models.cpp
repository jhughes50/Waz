/*!
*  Author:  Jason Hughes
*  Date:    July 2024
*  About:   A script to test model inference
*           using libtorch in the base network_manager class
*  Package: Waz
*/
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <torch/script.h>

#include "waz/network_manager.hpp"
#include "waz/depth.hpp"

int main()
{

    std::cout << "[MODEL-TEST] Initializing Depth Manager" << std::endl;

    DepthModeling dpa("/home/jason/config/networks.json", "depth");

    cv::Mat test_img = cv::imread("/home/jason/test/test-img.png", cv::IMREAD_COLOR);
    if (test_img.empty())
    {
        std::cout << "[MODEL-TEST] Cound not find test image" << std::endl;
        return -1;
    }
    else
    {
        std::cout << "[MODEL-TEST] Loaded test image" << std::endl;
    }
    at::Tensor result;
    result = dpa.inference(test_img);
    std::cout << "dimensions: " << result.ndimension() << std::endl;

    return 0;
}

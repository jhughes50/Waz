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

#include "waz/network_manager.hpp"

int main()
{

    std::cout << "[MODEL-TEST] Initializing Depth Manager" << std::endl;
    NetworkManager manager("/home/jason/config/networks.json","depth");
    std::cout << "[MODEL_TEST] Depth Manager Initialized" << std::endl;

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
   
    cv::Mat output = manager.inference(test_img);

    return 0;
}

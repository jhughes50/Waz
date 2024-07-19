/*!
* Author:  Jason Hughes
* Date:    July 2024
* About:   Test script for the cost map module
*
* Package: Waz
*/

#include <opencv2/opencv.hpp>
#include "waz/cost_map.hpp"
#include "waz/depth.hpp"
#include "waz/semantics.hpp"

int main(int argc, char **argv)
{

    google::InitGoogleLogging("/home/jason/.waz/");
    DepthManager depth("/home/jason/config/networks.json");
    SemanticsManager semantics("/home/jason/config/networks.json");
    
    CostMap cost_map("/home/jason/config/cost_map.json");
    
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
 
    cv::Mat depth_mat = depth.inference(test_img);
    cv::Mat semantics_mat = semantics.inference(test_img);

    cv::Mat cost_map_mat = cost_map.getCostMap(depth_mat, semantics_mat);

    cv::imshow("Cost Map", cost_map_mat);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}

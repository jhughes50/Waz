/*!
* Author:  Jason Hughes
* Date:    July 2024
* About:   Test script for the waz orchastrator module
*
* Package: Waz
*/

#include <opencv2/opencv.hpp>
#include "waz/waz.hpp"

int main(int argc, char **argv)
{

    google::InitGoogleLogging("/home/jason/.waz/");
   
    Waz waz("/home/jason/config/");

    cv::Mat test_img = cv::imread("/home/jason/test/imgs/test-img-1.png", cv::IMREAD_COLOR);
    if (test_img.empty())
    {
        std::cout << "[TEST] Cound not find test image" << std::endl;
        return -1;
    }
    else
    {
        std::cout << "[TEST] Loaded test image" << std::endl;
        cv::Size dims(1280, 720);
        cv::resize(test_img, test_img, dims, cv::INTER_AREA);
    }

    cv::Point goal(100,45);
    std::vector<cv::Point> path = waz(test_img, goal);
   
    std::cout << "[TEST] Path length: " << path.size() << std::endl;

    for (auto p : path)
    {
        std::cout << "path entry : " << p.x << ", " << p.y << std::endl;
    }

    cv::Mat cost_map = waz.getCostMap();
    cv::cvtColor(cost_map, cost_map, cv::COLOR_GRAY2BGR);

    cv::Mat drawn = waz.drawPath(test_img, path);
    
    cv::imshow("path", drawn);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}

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

    cv::Mat test_img = cv::imread("/home/jason/test/imgs/potholes.jpg", cv::IMREAD_COLOR);
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

    cv::Point goal(80,45);
    std::pair<std::vector<cv::Point>, std::vector<double>> path_angle_pair = waz(test_img, goal);
    
    std::vector<cv::Point> path = path_angle_pair.first;
    std::vector<double> angles = path_angle_pair.second;

    std::cout << "[TEST] Path length: " << path.size() << std::endl;
    for (const auto& element : angles) 
    {
        std::cout << element << " ";
    }
    std::cout << std::endl;

    cv::Mat cost_map = waz.getCostMap();
    cv::cvtColor(cost_map, cost_map, cv::COLOR_GRAY2BGR);

    cv::Mat cmd = waz.drawPath(cost_map, path, false);

    cv::imshow("cost map path", cmd);

    cv::Mat drawn = waz.drawPath(test_img, path);
    
    cv::imshow("path", drawn);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}

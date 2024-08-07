/*!
* Author:  Jason Hughes
* Date:    July 2024
* About:   Test script for the cost map module
*
* Package: Waz
*/

#include <opencv2/opencv.hpp>
#include "waz/api/cost_map.hpp"
#include "waz/api/depth.hpp"
#include "waz/api/semantics.hpp"
#include "waz/api/astar.hpp"

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
        cv::Size dims(1280, 720);
        cv::resize(test_img, test_img, dims, cv::INTER_AREA);

        cv::imshow("original", test_img);
        cv::waitKey(0);
        cv::destroyAllWindows();
    }
 
    std::cout << "[TEST] Inferenceing Depth Model" << std::endl;
    cv::Mat depth_mat = depth.inference(test_img);
    std::cout << "[TEST] Inferencing Semantics Model" << std::endl;
    cv::Mat semantics_mat = semantics.inference(test_img);

    std::cout << "[TEST] Getting Cost Map" << std::endl;
    cv::Mat cost_map_mat = cost_map.getCostMap(depth_mat, semantics_mat);

    std::cout << "[TEST] Showing Cost Map" << std::endl;
    cv::cvtColor(cost_map_mat, cost_map_mat, cv::COLOR_GRAY2BGR); 
    cv::Point start(80, 90);
    cv::Point goal(80, 45);

    cv::circle(cost_map_mat, start, 3, cv::Scalar(0,0,255), -1); 
    cv::circle(cost_map_mat, goal, 3, cv::Scalar(0,0,255), -1);

    cv::imshow("Cost Map", cost_map_mat);
    cv::waitKey(0);
    cv::destroyAllWindows();

    //AStarSearch astar;
    //cv::Point start(160, 180);
    //cv::Point goal(160, 90);

    std::vector<cv::Point> path;

    //path = astar.search(cost_map_mat, start, goal);

    std::cout << "[Test] Got path with size: " << path.size() << std::endl;

    return 0;
}

/*!
* Author:  Jason Hughes
* Date:    July 2024
* About:   A ROS wrapper for Waz
*
* Package: Waz
*/

#include "waz/ros/wazros.hpp"

WazRos::WazRos(Waz waz) : waz_(waz)
{
    ROS_INFO("[WAZROS] Initializing WazRos Node");
    image_sub_ = nh_.subscribe("/image", 1, &WazRos::imageCallback, this);

    img_pub_ = nh_.advertise<sensor_msgs::Image>("/path", 1);
    cost_map_pub_ = nh_.advertise<sensor_msgs::Image>("/cost_map", 1);
}

void WazRos::imageCallback(const sensor_msgs::CompressedImageConstPtr& msg)
{
    ROS_INFO_ONCE("[WAZROS] Image recieved");
    try
    {
        cv::Mat cv_image = cv::imdecode(cv::Mat(msg->data), cv::IMREAD_COLOR);

        std::pair<std::vector<cv::Point>, std::vector<double>> path_angle_pair = waz_(cv_image);
    
        std::vector<cv::Point> path = path_angle_pair.first;
        std::vector<double> angles = path_angle_pair.second;

        ROS_INFO_STREAM("[WAZROS] Got path with length: " << path.size() << std::endl);

        cv::Mat img_w_path = waz_.drawPath(cv_image, path);
        cv::Mat cost_map = waz_.getCostMap();

        sensor_msgs::ImagePtr img_path_msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", img_w_path).toImageMsg();
        img_pub_.publish(img_path_msg);

        sensor_msgs::ImagePtr cost_map_msg= cv_bridge::CvImage(std_msgs::Header(), "mono8", cost_map).toImageMsg();
        cost_map_pub_.publish(cost_map_msg);

    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("[WAZROS] CV Bridge exception: %s", e.what());
    }
}

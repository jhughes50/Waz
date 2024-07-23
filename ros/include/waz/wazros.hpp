/*!
* Author:  Jason Hughes
* Date:    July 2024
* About:   A ros wrapper for waz
*
* Package: Waz
*/

#ifndef WAZROS_HPP
#define WAZROS_HPP

#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>

class WazRos
{
    public:
        WazRos();
        
        void imageCallback(const sensor_msgs::ImageConstPtr& msg);

    private:
        ros::NodeHandle nh_;
        ros::Subscriber image_sub_;
};
#endif

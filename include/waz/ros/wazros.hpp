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

#include "waz/api/waz.hpp"

class WazRos
{
    public:
        WazRos(Waz waz);
        
        void imageCallback(const sensor_msgs::CompressedImageConstPtr& msg);

    private:
        ros::NodeHandle nh_;
        ros::Subscriber image_sub_;
        ros::Publisher img_pub_;
        ros::Publisher cost_map_pub_;

        Waz waz_;
};
#endif

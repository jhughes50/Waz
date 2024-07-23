/*!
* Author:  Jason Hughes
* Date:    July 2024
* About:   A ROS wrapper for Waz
*
* Package: Waz
*/

#include "waz/wazros.hpp"

WazRos::WazRos()
{
    image_sub_ = nh_.subscribe("/image", 1, &WazRos::imageCallback, this);
}

void WazRos::imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
    try
    {
        cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
        cv::Mat cv_image = cv_ptr->image;
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("[WAZROS] CV Bridge exception: %s", e.what());
    }
}

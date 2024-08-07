/*!
* Author:  Jason Hughes
* Date:    July 2024
* About:   A ROS node for Waz
*
* Package: Waz
*/

#include "waz/ros/wazros.hpp"

int main(int argc, char **argv)
{
    google::InitGoogleLogging("/home/jason/.waz/");
    ros::init(argc, argv, "wazros_node");

    Waz waz("/home/jason/ws/src/waz/config/");

    WazRos ros(waz);

    ros::spin();

    return 0;
}

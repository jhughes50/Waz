/*!
* Author:  Jason Hughes
* Date:    July 2024
* About:   A module to calculate linear and
*          angular velocities from the path
*
* Package: Waz
*/

#include "waz/velocity.hpp"

VelocityController::VelocityController(std::string path) : params_(path)
{
    params_.setParams();
}

double VelocityController::getLinearVelocity(const cv::Mat& cost_map, const cv::Point goal)
{
    int h = cost_map.rows
    int w = cost_map.cols

    num = (params_.max_speed * (h - goal.x);
    denom = std::sqrt(std::pow(w/2-goal.y, 2) + std::pow(h - goal.x, 2));

    return num /(double) denom;
}

double VelocityController::getAngularVelocity(const cv::Mat& cost_map, const cv::Point goal, const double fov)
{
    int h = cost_map.rows;
    int w = cost_map.cols;

    num = fov * (goal.y - w/(double)2);

    return num /(double) w;
}

void VelocityController::VelocityParams::setParams() noexcept
{
    hfov = params_map_["camera"]["hfov"].asInt();
    vfov = params_map_["camera"]["vfov"].asInt();

    max_speed = params_map_["robot"]["max_speed"].asInt();
}

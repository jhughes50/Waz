/*!
* Author:  Jason Hughes
* Date:    July 2024
* About:   A module to calculate linear and
*          angular velocities from the path
*
* Package: Waz
*/

#include "waz/velocity.hpp"

VelocityController::VelocityController(std::string path) : params_(path+"robot.json")
{
    params_.setParams();
    params_.getIntrinsics(path+"camera.yaml");
}

double VelocityController::getLinearVelocity(const cv::Mat& cost_map, const cv::Point& goal)
{
    int h = cost_map.rows;
    int w = cost_map.cols;

    double num = params_.max_speed * (h - goal.x);
    double denom = std::sqrt(std::pow(w/2-goal.y, 2) + std::pow(h - goal.x, 2));

    return num /(double) denom;
}

double VelocityController::getAngularVelocity(const cv::Mat& cost_map, const cv::Point& goal, const int fov)
{
    int h = cost_map.rows;
    int w = cost_map.cols;

    double num = fov * (goal.y - w/(double)2);

    return num /(double) w;
}

std::vector<cv::Point3d> VelocityController::pixelToCamera(const std::vector<cv::Point>& path)
{
    std::vector<cv::Point3d> path_cam_coords;
    for (cv::Point p : path)
    {
        cv::Mat p_mat = (cv::Mat_<double>(3,1) << p.x, p.y, 1.0);
        cv::Mat cam_coord = params_.K_inv * p_mat;

        cv::Point3d cam_p(cam_coord.at<double>(0,0), cam_coord.at<double>(1,0), cam_coord.at<double>(2,0));

        path_cam_coords.push_back(cam_p);
    }

    return path_cam_coords;
}

std::vector<double> VelocityController::calcAngles(const std::vector<cv::Point3d>& path)
{
    std::vector<double> angles;

    for (int i = 1; i < path.size(); ++i)
    {
        cv::Point3d p1 = path[i-1];
        cv::Point3d p2 = path[i];

        double dot_prod = (p1.x * p2.x) + (p1.y * p2.y) + (p1.z * p2.z);
        double mag1 = std::sqrt((p1.x*p1.x) + (p1.y*p1.y) + (p1.z*p1.z));
        double mag2 = std::sqrt((p2.x*p2.x) + (p2.y*p2.y) + (p2.z*p2.z));

        double angle = std::acos(dot_prod / (mag1 * mag2));    
        angles.push_back(angle);
    }

    return angles;
}

void VelocityController::VelocityParams::setParams() noexcept
{
    hfov = params_map_["camera"]["hfov"].asInt();
    vfov = params_map_["camera"]["vfov"].asInt();

    max_speed = params_map_["robot"]["max_speed"].asInt();
}

void VelocityController::VelocityParams::getIntrinsics(std::string path)
{
    cv::FileStorage fs(path, cv::FileStorage::READ);
    cv::FileNode cam_fm = fs["cam0"];
    cv::FileNode int_fm = cam_fm["intrinsics"];
    K = cv::Mat::eye(3,3, CV_64F);

    K.at<double>(0,0) = (double)int_fm[0];
    K.at<double>(1,1) = (double)int_fm[1];
    K.at<double>(0,2) = (double)int_fm[2];
    K.at<double>(1,2) = (double)int_fm[3];

    K_inv = K.inv();
}

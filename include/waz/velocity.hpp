/*!
* Author:  Jason Hughes
* Date:    July 2024
* About:   A module to go from a cost map
*          to velocity commands
*
* Package: Waz
*/

#ifndef VELOCITY_HPP
#define VELOCITY_HPP

#include <opencv2/opencv.hpp>
#include <cmath>
#include <string>
#include <vector>

#include "params.hpp"


class VelocityController
{
    public:
        VelocityController() = default;
        VelocityController(std::string path);

        double getLinearVelocity(const cv::Mat& cost_map, const cv::Point& goal);
        double getAngularVelocity(const cv::Mat& cost_map, const cv::Point& goal, const int fov);

        std::vector<cv::Point3d> pixelToCamera(const std::vector<cv::Point>& path);
        std::vector<double> calcAngles(const std::vector<cv::Point3d>& path);

        struct VelocityParams : public Params
        {
            using Params::Params;

            int hfov, vfov;
            int max_speed;
            cv::Mat K;
            cv::Mat K_inv;

            void setParams() noexcept;
            void getIntrinsics(std::string path);
        };

    private:
        
        VelocityParams params_;
};
#endif

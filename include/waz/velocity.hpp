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

#include "params.hpp"


class VelocityController
{
    public:
        VelocityController() = default;
        VelocityController(std::string path);

        void getLinearVelocity(const cv::Mat& cost_map; const cv::Point& goal);
        void getAngularVelocity(const cv::Mat& cost_map; const cv::Point&goal, const int fov);

        struct VelocityParams : public Params
        {
            using Params::Params;

            int hfov, vhov;
            int max_speed;

            void setParams() noexcept;
        };

    private:
    
        VelocityParams params_;
};
#endif

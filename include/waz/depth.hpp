/* Author:  Jason Hughes
*  Date:    June 2024
*  About:   A wrapper around a depth model.
*           This module will take in an image pass
*           it to the depth model, then do the proper
*           interpretation with relative depth.
*
*  Package: Waz
*/

#ifndef DEPTH_HPP
#define DEPTH_HPP

#include <string>
#include <opencv2/core.hpp>
#include <torch/script.h>

#include "network_manager.hpp"
#include "params.hpp"

class DepthModeling : protected NetworkManager
{
    public:
        DepthModeling(std::string params_path, std::string model_id = "depth");

        struct DepthParams: public Params
        {   
            using Params::Params;

            int patch;
            int channels;
            
            float mean_r, std_r;
            float mean_g, std_g;
            float mean_b, std_b;

            int input_height, input_width;

            void setParams() noexcept;
        };

        at::Tensor inference(cv::Mat& img);

    private:
        at::Tensor cvToTensor(const cv::Mat& mat, bool unsqueeze=false, uint8_t unsqueeze_dim=0) const noexcept;
        cv::Mat tensorToCv(const at::Tensor& tensor) const noexcept;

        void normalizeImage(cv::Mat& img);
        void resizeImage(cv::Mat& img);
    
        DepthParams params_;
};
#endif

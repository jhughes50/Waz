/* Author:  Jason Hughes
*  Date:    June 2024
*  About:   A wrapper around a semantic segmentation model
*           This module will take in an image and pass it to the model
*           then interpret the outpur for the cost map.
*
*  Package: Waz
*/

#ifndef SEMANTICS_HPP
#define SEMANTICS_HPP

#include <string>
#include <opencv2/core.hpp>
#include <torch/script.h>
#include <Eigen/Dense>

#include "network_manager.hpp"
#include "params.hpp"
#include "resize.hpp"
#include "normalize.hpp"
#include "interpolate.hpp"


class SemanticsManager : protected NetworkManager
{
    public:

        SemanticsManager(std::string path, std::string model_id = "semantics");
        cv::Mat inference(cv::Mat img);

        struct SemanticParams : public Params
        {
            using Params::Params;
            
            int channels;
            
            float mean_r, std_r;
            float mean_g, std_g;
            float mean_b, std_b;
            
            int input_height, input_width;

            static const int SIZE = 3;
            float mean[SIZE];
            float std[SIZE];

            void setParams() noexcept;
        };

        friend class SemanticsManagerTest;

    private:

        Eigen::MatrixXi tensorToEigen(const at::Tensor& tensor) const noexcept;
        cv::Mat tensorToCv(const at::Tensor& tensor) const noexcept;

        at::Tensor postProcess(at::Tensor& result);

        SemanticParams params_;
        Resize resize_;
        Normalize normalize_;
        Interpolate interpolate_;
};
#endif

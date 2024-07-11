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

#include "network_manager.hpp"
#include "params.hpp"

class SemanticsManager : protected NetworkManager
{
    public:

        SemanticsManager(std::string path, std::string model_id = "semantics");

        struct SemanticParams : public Params
        {
            using Params::Params;
            
            int channels;
            
            float mean_r, std_r;
            float mean_g, std_g;
            float mean_b, std_b;
            
            int input_height, input_width;

            void setParams() noexcept;
        };

        friend class SemanticsManagerTest;

    private:

        at::Tensor cvToTensor(const cv::Mat& mat, bool unsqueeze = false, uint8_t unsqueeze_dim = 0);
        
        SemanticParams params_;
};
#endif

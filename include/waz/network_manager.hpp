/* Author:  Jason Hughes
*  Date:    June 2024
*  About:   A base class for the shared functions of the 
*           two network wrappers. This will get the model and pass
*           it back to the wrapper and take car of inference.
*  Package: Waz
*/

#ifndef NETWORK_MANAGER_HPP
#define NETWORK_MANAGER_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include <opencv2/opencv.hpp>
#include <torch/script.h>
#include <json/json.h>
#include <glog/logging.h>

#include "params.hpp"

class NetworkManager
{
    public:
        //NetworkManager();
        NetworkManager(std::string params_path, std::string model_id);

        struct NetworkParams : public Params
        {    
            using Params::Params;
            
            std::string model_path;
            std::string device;
            int channels;

            void setParams(std::string model_id) noexcept;
        };

    protected:
        // Class functions to be inherited
        void load(const std::string path) noexcept;
        at::Tensor forward(const at::Tensor img) noexcept;
        at::Tensor cvToTensor(const cv::Mat& mat, bool unsqueeze=false, uint8_t unsqueeze_dim=0) const noexcept;

    private:
        // Class varaibles
        // Torch variables
        torch::jit::script::Module module_;

        // Parameter varaibles
        NetworkParams params_;
        std::string model_id_;
};
#endif 

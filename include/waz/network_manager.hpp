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

#include <opencv2/opencv.hpp>
#include <torch/script.h>
#include <json/json.h>
#include <glog/logging.h>

#include "params.hpp"

class NetworkManager
{
    public:
        NetworkManager(std::string params_path);

        struct NetworkParams : Params(params_path + "/network_manager.json")
        {    
            std::string model_path;
        };

    protected:
        void load(std::string path);
        void inference(cv::Mat img);

    private:
        void setParams();

        torch::jit::script::Module module_;
        NetworkParams params_;
        std::string model_id_;
};
#endif

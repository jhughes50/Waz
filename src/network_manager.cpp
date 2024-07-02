/* Author:  Jason Hughes
*  Date:    June 2024
*  About:   Network Manager logic
*  Package: Waz
*/

#include "waz/network_manager.hpp"

NetworkManager::NetworkManager(std::string params_path, std::string model_id) : params_(params_path)
{
    // have the constructor load the params json and set them 
    google::InitGoogleLogging(".waz/");
    model_id_ = model_id;
    params_.setParams(model_id);
    //device_ = at::kCUDA;
    load(params_.model_path);
}

void NetworkManager::load(const std::string model_path) noexcept
{
    try
    {
        module_ = torch::jit::load(model_path, at::kCUDA);
        module_.eval();
    }
    catch (const c10::Error &e)
    {
       LOG(FATAL) << "Model at " << model_path << " was not found because " << e.what() <<", exiting.";      
    }
    LOG(INFO) << "Model loaded succesfully at " << model_path;
}

at::Tensor NetworkManager::forward(const at::Tensor img) noexcept
{
    std::vector<torch::jit::IValue> input;
    input.push_back(img);

    at::Tensor output = module_.forward(input).toTensor();

    return output;
}

at::Tensor NetworkManager::cvToTensor(const cv::Mat& img, bool unsqueeze, uint8_t unsqueeze_dim) const noexcept
{
    auto options = at::TensorOptions().device(at::kCUDA,1).requires_grad(false);
    at::Tensor tensor_img = torch::from_blob(img.data, { img.rows, img.cols, params_.channels }, options);

    if (unsqueeze)
    {
        tensor_img.unsqueeze_(unsqueeze_dim);
    }

    return tensor_img;
}

void NetworkManager::NetworkParams::setParams(std::string model_id) noexcept
{
    model_path = params_map_[model_id]["path"].asString();
    device = params_map_["torch"]["device"].asString();
    channels = params_map_["torch"]["channels"].asInt();
}







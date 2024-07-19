/* Author:  Jason Hughes
*  Date:    June 2024
*  About:   Network Manager logic
*  Package: Waz
*/

#include "waz/network_manager.hpp"


NetworkManager::NetworkManager(std::string params_path, std::string model_id) : params_(params_path)
{
    // have the constructor load the params json and set them 
    // google::InitGoogleLogging("/home/jason/.waz/");
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

    //at::Tensor output = module_.forward(input).toTensor();
    at::Tensor output = module_.forward(input).toTuple()->elements()[0].toTensor();
    return output;
}

at::Tensor NetworkManager::cvToTensor(const cv::Mat& mat, bool unsqueeze, uint8_t unsqueeze_dim) const noexcept
{
    // make sure the image is continuous in mem
    cv::Mat c_img = mat.isContinuous() ? mat : mat.clone();

    auto options = at::TensorOptions().device(at::kCUDA, 1).requires_grad(false);
    at::Tensor tensor_img = torch::from_blob(c_img.data, {c_img.rows, c_img.cols, params_.channels});//, options);
    
    tensor_img = tensor_img.permute({2, 0, 1});
    tensor_img = tensor_img.to(torch::kFloat32);
    if (unsqueeze)
    {
        tensor_img.unsqueeze_(unsqueeze_dim);
    }
    tensor_img = tensor_img.to(at::kCUDA);
    
    return tensor_img;
}

void NetworkManager::NetworkParams::setParams(std::string model_id) noexcept
{
    model_path = params_map_[model_id]["path"].asString();
    device = params_map_["torch"]["device"].asString();
    channels = params_map_["torch"]["channels"].asInt();
}







/*!
* Author:  Jason Hughes
* Date:    July 2024
* About:   Wrapper around the segformer model 
*
* Package: Waz
*/

#include "waz/semantics.hpp"

SemanticsManager::SemanticsManager(std::string params_path, std::string model_id) : NetworkManager(params_path, model_id), params_(params_path)
{
    params_.setParams();
}
    
void SemanticsManager::SemanticParams::setParams() noexcept
{
    channels = params_map_["torch"]["channels"].asInt();
    
    mean_r = params_map_["semantics"]["mean_r"].asFloat();
    mean[0] = mean_r;
    mean_g = params_map_["semantics"]["mean_g"].asFloat();
    mean[1] = mean_g;
    mean_b = params_map_["semantics"]["mean_b"].asFloat();
    mean[2] = mean_b;

    std_r = params_map_["semantics"]["std_r"].asFloat();
    mean[0] = std_r;
    std_g = params_map_["semantics"]["std_g"].asFloat();
    mean[1] = std_g;
    std_b = params_map_["semantics"]["std_b"].asFloat();
    mean[2] = std_b;

    input_height = params_map_["semantics"]["input_height"].asInt();
    input_width = params_map_["semantics"]["input_width"].asInt();
}

Eigen::MatrixXi SemanticsManager::tensorToEigen(const at::Tensor& tensor) const noexcept
{
    int rows = tensor.size(0);
    int cols = tensor.size(1);
    
    Eigen::MatrixXi mat;

    std::memcpy(mat.data(), tensor.data_ptr<int>(), sizeof(int) * rows * cols);

    return mat;
}

cv::Mat SemanticsManager::tensorToCv(const at::Tensor& tensor) const noexcept
{
    int rows = tensor.size(0);
    int cols = tensor.size(1);

    cv::Mat img(rows, cols, CV_8UC1);
    std::memcpy(img.data, tensor.data_ptr<uint8_t>(), sizeof(torch::kUInt8) * tensor.numel());

    return img;
}

at::Tensor SemanticsManager::postProcess(at::Tensor& result)
{
    result.squeeze(0);
    return torch::argmax(result, 0).to(torch::kUInt8);
}

cv::Mat SemanticsManager::inference(cv::Mat& img)
{   
    int h = img.rows;
    int w = img.cols;

    at::Tensor input, result;
    normalize_(img, params_.mean, params_.std);
    resize_(img, params_.input_width, params_.input_height);

    input = cvToTensor(img, true, 0);

    result = forward(input).to(at::kCPU);
    result = interpolate_(result, h, w);
    result = postProcess(result);

    return tensorToCv(result);
}



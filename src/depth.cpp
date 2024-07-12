/*!
* Author:  Jason Hughes
* Date:    July 2024
* About:   Depth modeling logic,
*
* Package: Waz
*/

#include "waz/depth.hpp"

DepthManager::DepthManager(std::string params_path, std::string model_id) : NetworkManager(params_path, model_id), params_(params_path)
{
    params_.setParams();
}

void DepthManager::normalizeImage(cv::Mat& img)
{
    normalize_(img, params_.mean, params_.std, true);
}

void DepthManager::resizeImage(cv::Mat& img)
{
    Resize resize(params_.input_width, params_.input_height, true, params_.patch);
    resize(img);
}

Eigen::MatrixXf DepthManager::tensorToEigen(const at::Tensor& tensor) const noexcept
{
    int rows = tensor.size(0);
    int cols = tensor.size(1);
    
    Eigen::MatrixXf mat(rows, cols);

    std::memcpy(mat.data(), tensor.data_ptr<float>(), sizeof(float) * rows * cols);

    return mat;
}

Eigen::MatrixXf DepthManager::inference(cv::Mat& img)
{
    at::Tensor input, result;
    normalizeImage(img);
    resizeImage(img);
    input = cvToTensor(img, true, 0);

    result = forward(input).to(at::kCPU);
    result = result.squeeze(0);
    Eigen::MatrixXf result_mat = tensorToEigen(result);

    return result_mat;
}

void DepthManager::DepthParams::setParams() noexcept
{
    patch = params_map_["depth"]["patch"].asInt();
    channels = params_map_["torch"]["channels"].asInt();
    
    mean_r = params_map_["depth"]["mean_r"].asFloat();
    mean[0] = mean_r;
    mean_g = params_map_["depth"]["mean_g"].asFloat();
    mean[1] = mean_g;
    mean_b = params_map_["depth"]["mean_b"].asFloat();
    mean[2] = mean_b;

    std_r = params_map_["depth"]["std_r"].asFloat();
    std[0] = std_r;
    std_g = params_map_["depth"]["std_g"].asFloat();
    std[1] = std_g;
    std_b = params_map_["depth"]["std_b"].asFloat();
    std[2] = std_b;

    input_height = params_map_["depth"]["input_height"].asInt();
    input_width = params_map_["depth"]["input_width"].asInt();
}

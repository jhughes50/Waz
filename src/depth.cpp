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

void DepthManager::postProcess(at::Tensor& tensor)
{
    at::Tensor min = torch::min(tensor);
    at::Tensor max = torch::max(tensor);

    tensor = (tensor - min) / (max - min) * 255.0;
    tensor = tensor.to(torch::kUInt8);
}

cv::Mat DepthManager::sobel(cv::Mat& img)
{
    cv::Mat grad_x, grad_y, grad;

    // Am I doing a harsh type conversion here?
    cv::Sobel(img, grad_x, CV_64F, 1, 0, params_.filter_size);
    cv::Sobel(img, grad_y, CV_64F, 0, 1, params_.filter_size);

    cv::magnitude(grad_x, grad_y, grad);
    cv::normalize(grad, grad, 0, 255, cv::NORM_MINMAX, CV_8U);

    return grad;
}

Eigen::MatrixXi DepthManager::tensorToEigen(const at::Tensor& tensor) const noexcept
{
    int rows = tensor.size(0);
    int cols = tensor.size(1);

    Eigen::MatrixXi mat(rows, cols);

    std::memcpy(mat.data(), tensor.data_ptr<uint8_t>(), sizeof(int) * rows * cols);

    return mat;
}

cv::Mat DepthManager::tensorToCv(const at::Tensor& tensor) const noexcept
{
    int rows = tensor.size(0);
    int cols = tensor.size(1);

    cv::Mat img(rows, cols, CV_8UC1);
    std::memcpy(img.data, tensor.data_ptr<uint8_t>(), sizeof(torch::kUInt8) * tensor.numel());

    return img;
}

cv::Mat DepthManager::inference(cv::Mat img)
{
    int h = img.rows;
    int w = img.cols;

    at::Tensor input, result;
    normalizeImage(img);
    resizeImage(img);
    input = cvToTensor(img, true, 0);

    result = forward(input).to(at::kCPU);
    result = result.squeeze(0);
    postProcess(result);
    cv::Mat result_cv = tensorToCv(result);
    interpolate_(result_cv, h, w);

    cv::Mat grad = sobel(result_cv);

    return grad;
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
    filter_size = params_map_["depth"]["filter"].asInt();
}

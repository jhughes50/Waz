/*!
* Author:  Jason Hughes
* Date:    July 2024
* About:   Depth modeling logic,
*
* Package: Waz
*/

#include "waz/depth.hpp"
#include "waz/resize.hpp"

DepthModeling::DepthModeling(std::string params_path, std::string model_id) : NetworkManager(params_path, model_id), params_(params_path)
{
    params_.setParams();
}

void DepthModeling::normalizeImage(cv::Mat& img)
{
    // convert from BGR to RGB
    cv::cvtColor(img, img, cv::COLOR_RGB2BGR);
    img.convertTo(img, 5, 1.0/255.0);

    // divide by mean subtract std
    cv::Scalar mean_values(params_.mean_r, params_.mean_g, params_.mean_b);
    cv::Scalar std_values(params_.std_r, params_.std_g, params_.std_b);

    cv::subtract(img, mean_values, img);
    cv::divide(img, std_values, img);
}

void DepthModeling::resizeImage(cv::Mat& img)
{
    Resize resize(params_.input_width, params_.input_height, true, params_.patch);
    resize(img);
}

at::Tensor DepthModeling::cvToTensor(const cv::Mat& mat, bool unsqueeze, uint8_t unsqueeze_dim) const noexcept
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
    std::cout << tensor_img.sizes()[0] << " " << tensor_img.sizes()[1] << " " << tensor_img.sizes()[2] << " " << tensor_img.sizes()[3] << std::endl;
    tensor_img = tensor_img.to(at::kCUDA);
    
    return tensor_img;
}

cv::Mat DepthModeling::tensorToCv(const at::Tensor& tensor) const noexcept
{

}

at::Tensor DepthModeling::inference(cv::Mat& img)
{
    at::Tensor input, result;
    normalizeImage(img);
    resizeImage(img);
    input = cvToTensor(img, true, 0);

    result = forward(input);

    return result;
}

void DepthModeling::DepthParams::setParams() noexcept
{
    patch = params_map_["depth"]["patch"].asInt();
    channels = params_map_["torch"]["channels"].asInt();
    
    mean_r = params_map_["depth"]["mean_r"].asFloat();
    mean_g = params_map_["depth"]["mean_g"].asFloat();
    mean_b = params_map_["depth"]["mean_b"].asFloat();

    std_r = params_map_["depth"]["std_r"].asFloat();
    std_g = params_map_["depth"]["std_g"].asFloat();
    std_b = params_map_["depth"]["std_b"].asFloat();

    input_height = params_map_["depth"]["input_height"].asInt();
    input_width = params_map_["depth"]["input_width"].asInt();
}

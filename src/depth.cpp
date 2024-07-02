/*!
* Author:  Jason Hughes
* Date:    July 2024
* About:   Depth modeling logic,
*
* Package: Waz
*/

#include "waz/depth.hpp"

DepthModeling::DepthModeling(std::string params_path, std::string model_id)
{

}

cv::Mat DepthModeling::normalizeImage(cv::Mat& img)
{

}

at::Tensor DepthModeling::cvToTensor(const cv::Mat& mat, bool unsqueeze, uint8_t, unsqueeze_dim) noexcept
{
    auto options = at::TensorOptions().device(at::kCUDA, 1).requires_grad(false);
    at::Tensor tensor_img = torch::from_blob(mat.data, {mat.rows, mat.cols, params_.channels}, options);

    if (unsqueeze)
    {
        tensor_img.unsqueeze_(unsqueeze_dim);
    }

    return tensor_img;
}

cv::Mat DepthModeling::tensorToCv(const at::Tensor& tensor)
{

}

void DepthModeling::DepthParams::setParams() noexcept
{
    patch = params_map_["depth"]["patch"].asString();
    channels = params_map_["torch"]["channels"].asInt();
}

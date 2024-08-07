/*!
* Author:  Jason Hughes
* Date:    July 2024
* About:   Test the pipeline for the semantic
*          segmentation model
*
* Package: Waz
*/

#include "waz/apidepth.hpp"
#include "waz/model.hpp"
#include "waz/cost_map.hpp"

void print_tensor_dtype(const torch::Tensor& tensor) 
{
    auto dtype = tensor.scalar_type();
    
    std::cout << "Tensor data type: ";
    
    switch (dtype) 
    {
        case torch::kFloat32:
            std::cout << "Float32";
            break;
        case torch::kFloat64:
            std::cout << "Float64";
            break;
        case torch::kInt32:
            std::cout << "Int32";
            break;
        case torch::kInt64:
            std::cout << "Int64";
            break;
        case torch::kUInt8:
            std::cout << "UInt8";
            break;
        case torch::kInt8:
            std::cout << "Int8";
            break;
        case torch::kInt16:
            std::cout << "Int16";
            break;
        case torch::kBool:
            std::cout << "Bool";
            break;
        // Add more cases as needed
        default:
            std::cout << "Unknown";
    }
    
    std::cout << std::endl;
}


int main(int argc, char **argv)
{
    DepthManager depth("/home/jason/config/networks.json", "depth");
    CostMap cost_map("/home/jason/config/cost_map.json");

    cv::Mat test_img = cv::imread("/home/jason/test/imgs/test-img-1.png", cv::IMREAD_COLOR);
    if (test_img.empty())
    {
        std::cout << "[TEST] Cound not find test image" << std::endl;
        return -1;
    }
    else
    {
        std::cout << "[TEST] Loaded test image" << std::endl;
    }
    std::cout << "[TEST] Running image through depth model..." << std::endl;
    cv::Mat result = depth.inference(test_img);
    cv::Mat kern = cost_map.kernalizeMask(result, Model::DEPTH);
    std::cout << kern.rows << "  " << kern.cols << std::endl;
    cv::imshow("depth", kern);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}

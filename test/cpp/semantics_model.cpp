/*!
* Author:  Jason Hughes
* Date:    July 2024
* About:   Test the pipeline for the semantic
*          segmentation model
*
* Package: Waz
*/

#include "waz/semantics.hpp"

class SemanticsManagerTest
{
    public:
        SemanticsManagerTest() : manager_("/home/jason/config/networks.json", "semantics") {}

        cv::Mat test(cv::Mat& img)
        {
            
        }


        SemanticsManager manager_;       
};


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
    SemanticsManager semantics("/home/jason/config/networks.json", "semantics");

    cv::Mat test_img = cv::imread("/home/jason/test/imgs/test-img.png", cv::IMREAD_COLOR);
    if (test_img.empty())
    {
        std::cout << "[TEST] Cound not find test image" << std::endl;
        return -1;
    }
    else
    {
        std::cout << "[TEST] Loaded test image" << std::endl;
    }
    std::cout << "[TEST] Running image" << std::endl;
    at::Tensor result = semantics.inference(test_img);
    result = result.squeeze(0);
    at::Tensor labels = torch::argmax(result, 0);
    std::cout << "[TEST] Ran image successfully" << std::endl;
    //std::cout << "[TEST] Result has rows: " << result.rows() << " and cols: " << result.cols() << std::endl;
    std::cout << "[TEST] Number of Dimensions in tensor: " << result.dim() << std::endl;
    std::cout << "[TEST] Tensor dims: " << result.size(0) << " " << result.size(1) << " " << result.size(2) << " " << std::endl;
    print_tensor_dtype(result);
    std::cout << "[TEST] labels: " << labels.dim() << std::endl;
    std::cout << labels << std::endl;
    //std::cout << result.select(0,0) << std::endl;
    return 0;
}

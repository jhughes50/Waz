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
    std::cout << "[TEST] Running image" << std::endl;
    cv::Mat result = semantics.inference(test_img);
    std::cout << result.col(128) << std::endl;
    std::cout << "Dims: " << result.dims << std::endl;
    std::cout << "Img with: " << result.rows << " by " << result.cols << std::endl;

    return 0;
}

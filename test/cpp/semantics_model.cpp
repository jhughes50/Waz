/*!
* Author:  Jason Hughes
* Date:    July 2024
* About:   Test the pipeline for the semantic
*          segmentation model
*
* Package: Waz
*/

#include "waz/semantics.hpp"

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
    std::cout << "[TEST] Ran image successfully" << std::endl;
    //std::cout << "[TEST] Result has rows: " << result.rows() << " and cols: " << result.cols() << std::endl;
    std::cout << "[TEST] Number of Dimensions in tensor: " << result.dim() << std::endl;
    std::cout << "[TEST] Tensor dims: " << result.size(0) << " " << result.size(1) << " " << result.size(2) << " " << result.size(3) << std::endl;
    return 0;
}

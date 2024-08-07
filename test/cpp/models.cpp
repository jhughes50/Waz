/*!
*  Author:  Jason Hughes
*  Date:    July 2024
*  About:   A script to test model inference
*           using libtorch in the base network_manager class
*  Package: Waz
*/
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <torch/script.h>
#include <glog/logging.h>
#include <gtest/gtest.h>

#include "waz/api/network_manager.hpp"
#include "waz/api/depth.hpp"

class DepthManagerTest
{
    protected:
        DepthManager manager_;
        cv::Mat test_img_;

    public:
        DepthManagerTest(std::string path, std::string model_id, cv::Mat& test_img);

        void testCvToTensor();
        void testResizeImage();
};

DepthManagerTest::DepthManagerTest(std::string path, std::string model_id, cv::Mat& test_img) : manager_(path, model_id)
{
    test_img_ = test_img;
}

void DepthManagerTest::testCvToTensor()
{
    at::Tensor tensor = manager_.cvToTensor(test_img_, true, 0);

    LOG(INFO) << "Running cv to tensor conversion test";
    ASSERT_EQ(1, tensor.size(0));
    ASSERT_EQ(3, tensor.size(1));
    ASSERT_EQ(720, tensor.size(2));
    ASSERT_EQ(1280, tensor.size(3));
    LOG(INFO) << "Finished conversion test sucessfully";
}

void DepthManagerTest::testResizeImage()
{
    manager_.resizeImage(test_img_);

    LOG(INFO) << "Running resize image test";
    ASSERT_EQ(518, test_img_.rows);
    ASSERT_EQ(924, test_img_.cols);
    LOG(INFO) << "Finished resize image test sucessfully";
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    //google::InitGoogleLogging(".waz/");

    cv::Mat test_img = cv::imread("/home/jason/test/test-img.png", cv::IMREAD_COLOR);
    if (test_img.empty())
    {
        std::cout << "Cound not find test image" << std::endl;
        return -1;
    }
    else
    {
        std::cout << "[TEST] Loaded test image" << std::endl;
    }
   
    DepthManagerTest dmt("/home/jason/config/networks.json", "depth", test_img);
    FLAGS_alsologtostderr = 1; 
    dmt.testCvToTensor();
    dmt.testResizeImage();

    return 0;
}

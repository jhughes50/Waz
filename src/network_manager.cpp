/* Author:  Jason Hughes
*  Date:    June 2024
*  About:   Network Manager logic
*  Package: Waz
*/

#include "waz/network_manager.hpp"

NetworkManager::NetworkManager(std::string params_path, std::string model_id)
{
    // have the constructor load the params json and set them 
    google::InitGoogleLogging(".waz/")
    model_id_ = model_id;
    setParams()
}

void NetworkManager::load(std::string model_path)
{
    try
    {
        module_ = torch::jit::load(model_path);
    }
    catch (const c10::Error &e)
    {
       LOG(FATAL) << "Model at " << model_path << " was not found, exiting.";      
    }
}

void NetworkManager::inference(cv::Mat img)
{

}

void NetworkManager::setParams()
{
    std::string model_path = params_map_[model_id_]["path"];
}

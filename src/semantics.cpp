/*!
* Author:  Jason Hughes
* Date:    July 2024
* About:   Wrapper around the segformer model 
*
* Package: Waz
*/

#include "waz/semantics.hpp"

SemanticsManager::SemanticsManager(std::string params_path, std::string model_id) : NetworkManager(params_path, model_id), params_(params_path);
{
    params_.setParams();
}
    
void SemanticsManager::SemanticsParams::setParams() noexcept
{
    channels = params_map_["torch"]["channels"].asInt();
    
    mean_r = params_map_["semantics"]["mean_r"].asFloat();
    mean_g = params_map_["semantics"]["mean_g"].asFloat();
    mean_b = params_map_["semantics"]["mean_b"].asFloat();

    std_r = params_map_["semantics"]["std_r"].asFloat();
    std_g = params_map_["semantics"]["std_g"].asFloat();
    std_b = params_map_["semantics"]["std_b"].asFloat();

    input_height = params_map_["semantics"]["input_height"].asInt();
    input_width = params_map_["semantics"]["input_width"].asInt();
}


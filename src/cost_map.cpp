/*!
* Author:  Jason Hughes
* Date:    July 2024
* About:   Cost Map Logic
*
* Package: Waz
*/

#include "waz/cost_map.hpp"

CostMap::CostMap(std::string path) : params_(path)
{

}

void CostMap::CostMapParams::setParams() noexcept
{
    width = params_map_["width"].asInt();
    height = params_map_["height"].asInt();

    int num_classes = params_map_["num_classes"].asInt();

    for (uint8_t i=0; i < num_classes; ++i)
    {
        std::string i_str = std::to_string(i);
        std::string label = params_map_["classes"][i_str]["id"].asString();
        uint8_t cost = params_map_["classes"][i_str]["cost"].asInt();

        label_map[i] = LabelMap(label, cost, i);
    }
}

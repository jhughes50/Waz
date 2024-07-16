/*!
* Author:  Jason Hughes
* Date:    July 2024
* About:   A struct to hold the id cost and label of 
*          each segformer class.
*
* Package: Waz
*/
#ifndef LABEL_MAP_HPP
#define LABEL_MAP_HPP

#include <string>

struct LabelMap
{
    LabelMap() = default;
    LabelMap(std::string, uint8_t, uint8_t);

    std::string label;
    uint8_t cost;
    uint8_t id;
};
#endif

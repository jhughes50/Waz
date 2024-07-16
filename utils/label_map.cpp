/*!
* Author:  Jason Hughes
* Date:    July 2024
* About:   Constructor definition 
*
* Package: Waz
*/

#include "waz/label_map.hpp"

LabelMap::LabelMap(std::string l, uint8_t c, uint8_t i)
{
    label = l;
    cost = c;
    id = i;
}

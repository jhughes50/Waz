/*!
* Author:  Jason Hughes
* Date:    June 2024
* About:   Logic for Params base strurct
* Package: Waz
*/

#include "waz/utils/params.hpp"

Params::Params(std::string path)
{
    std::ifstream params_file(path, std::ifstream::binary);
    params_file >> params_map_;
}

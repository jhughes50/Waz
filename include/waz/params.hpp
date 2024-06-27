/*!
 * Author:  Jason Hughes
 * Date:    June 2024
 * About:   A base class for Parameter functions
 * Package: Waz
 *
*/
#ifndef PARAMS_HPP
#define PARAMS_HPP

#include <iostream>
#include <fstream>
#include <json/json.h>

struct Params
{
    Params(std::string path);
    Json::Value params_map_;
};
#endif

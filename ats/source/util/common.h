//
// Created by xtgu on 7/10/19.
//

#ifndef MUSE_COMMON_H
#define MUSE_COMMON_H
#include "../types/errorcode.h"
#include <vector>
#include <string>
#include <iostream>
#include <json.hpp>


namespace ats {

    std::vector<std::string> str_split(const std::string& str, const std::string& delim);


    void input_type_check(const nlohmann::json &param, const std::string &need_type_name);



}


#endif //MUSE_COMMON_H

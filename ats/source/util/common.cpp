//
// Created by xtgu on 7/10/19.
//
#include "common.h"

namespace ats{
    using namespace std;

    vector<std::string> str_split(const string& str, const string& delim)
    {
        vector<std::string> tokens;
        size_t prev = 0, pos = 0;
        do
        {
            pos = str.find(delim, prev);
            if (pos == std::string::npos) pos = str.length();
            std::string token = str.substr(prev, pos-prev);
            if (!token.empty()) tokens.push_back(token);
            prev = pos + delim.length();
        }
        while (pos < str.length() && prev < str.length());
        return tokens;
    }

    void input_type_check(const nlohmann::json &param, const std::string &need_type_name) {

        if (need_type_name == "int") {
            if (param.is_number_integer() or param.is_number_unsigned()) {
                return;
            }
        }

        if (need_type_name == "double" or need_type_name == "float") {

            if (param.is_number_integer() or param.is_number_unsigned()) {
                return;
            }
            if(param.is_number_float()) {
                return;

            }

        }

        throw ErrorCategory(ErrorCode::WRONG_INPUT_TYPE);

    }



}
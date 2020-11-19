//
// Created by Rong Zhou on 2018-08-03.
//

#ifndef ATS_TIMEHELPER_H
#define ATS_TIMEHELPER_H

#include <chrono>
#include <string>
#include "sys/time.h"

namespace ats {

    std::chrono::system_clock::time_point string_to_time_point(const std::string &str);

    std::chrono::system_clock::time_point int_string_to_time_point(const std::string &str);

    std::string time_point_to_string(std::chrono::system_clock::time_point tp);

    std::chrono::system_clock::time_point string_to_time_date(const std::string &str);

    std::string date_to_string(std::chrono::system_clock::time_point dt);

    std::string get_current_date();

}

#endif //ATS_TIMEHELPER_H

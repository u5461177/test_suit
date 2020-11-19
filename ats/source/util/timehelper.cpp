//
// Created by Rong Zhou on 2018-08-03.
//

#include "timehelper.h"
#include <sstream>
#include <iomanip>

namespace ats {

    using namespace std;
    using namespace std::chrono;
    using time_point = std::chrono::system_clock::time_point;

    time_point string_to_time_point(const string &str) {
        time_t now = time(nullptr);
        tm t = *localtime(&now);
        istringstream(str) >> get_time(&t, "%H:%M:%S");
        return system_clock::from_time_t(mktime(&t));
    }

    time_point string_to_time_date(const string &str) {
        time_t now = time(nullptr);
        tm t = *localtime(&now);
        istringstream(str) >> get_time(&t, "%Y%m%d");
        return system_clock::from_time_t(mktime(&t));
    }


    time_point int_string_to_time_point(const string &str) {
        time_t now = time(nullptr);
        tm t = *localtime(&now);
        istringstream(str) >> get_time(&t, "%H%M%S");
        return system_clock::from_time_t(mktime(&t));
    }

    string time_point_to_string(time_point tp) {
        time_t tt = system_clock::to_time_t(tp);
        ostringstream oss;
        oss << put_time(localtime(&tt), "%T");
        return oss.str();
    }

    std::string date_to_string(std::chrono::system_clock::time_point dt) {
        time_t tt = system_clock::to_time_t(dt);
        ostringstream oss;
        oss << put_time(localtime(&tt), "%Y%m%d");
        return oss.str();
    }

    string get_current_date() {
        time_t nowtime;
        nowtime = time(nullptr);
        char tmp[64];
        strftime(tmp, sizeof(tmp), "%Y%m%d", localtime(&nowtime));
        return tmp;
    }

}
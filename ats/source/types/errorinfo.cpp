//
// Created by xhshu on 8/2/18.
//

#include "errorinfo.h"

namespace ats {

    using namespace std;
    using json = nlohmann::json;

    vector<string> ErrorInfo::to_record(const ErrorInfo &error_info) {
        return vector<string>{};
    }

    ErrorInfo::ErrorInfo(XTPRI *error_info) {
        error_id = error_info->error_id;
        error_msg = error_info->error_msg;
    }

    ErrorInfo::ErrorInfo(const ErrorInfo &other) {
        error_id = other.error_id;
        error_msg = other.error_msg;
    }

    json ErrorInfo::to_json(const ats::ErrorInfo &error_info) {
        json result;
        result["error_id"] = error_info.error_id;
        result["error_msg"] = error_info.error_msg;
        return result;
    }

}
//
// Created by xhshu on 8/2/18.
//

#ifndef ATS_ERRORINFO_H
#define ATS_ERRORINFO_H

#include <json.hpp>
#include <xtp_api_struct.h>

namespace ats {

    struct ErrorInfo {
        static std::vector<std::string> to_record(const ErrorInfo &error_info);

        static nlohmann::json to_json(const ErrorInfo &error_info);

        explicit ErrorInfo(XTPRI *error_info);

        ErrorInfo() = default;

        ErrorInfo(const ErrorInfo &other);

        int32_t error_id;
        std::string error_msg;
    };
}


#endif //ATS_ERRORINFO_H

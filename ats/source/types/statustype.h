//
// Created by Rong Zhou on 2018-07-31.
//

#ifndef ATS_STATUSTYPE_H
#define ATS_STATUSTYPE_H

#include <string>

namespace ats {

    enum class StatusType {
        IN_PROGRESS,
        FINISH,
        EXPIRE,
        PAUSED,
        STOP,
        WARNING,
        DATA_ERROR,
        TASK_ERROR,
        SYSTEM_ERROR,
        PROCESS_INTERRUPTED,
        UNKNOWN
    };

    StatusType string_to_status_type(const std::string &str);

    std::string status_type_to_string(StatusType status_type);

};

#endif //ATS_STATUSTYPE_H
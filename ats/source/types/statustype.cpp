//
// Created by Rong Zhou on 2018-07-31.
//

#include "statustype.h"

namespace ats {

    using namespace std;

    StatusType string_to_status_type(const string &str) {
        if (str == "IN_PROGRESS") {
            return StatusType::IN_PROGRESS;
        }
        if (str == "FINISH") {
            return StatusType::FINISH;
        }
        if (str == "PAUSED") {
            return StatusType::PAUSED;
        }
        if (str == "EXPIRE") {
            return StatusType::EXPIRE;
        }
        if (str == "STOP") {
            return StatusType::STOP;
        }
        if (str == "WARNING") {
            return StatusType::WARNING;
        }
        if (str == "DATA_ERROR") {
            return StatusType::DATA_ERROR;
        }
        if (str == "TASK_ERROR") {
            return StatusType::TASK_ERROR;
        }
        if (str == "SYSTEM_ERROR") {
            return StatusType::SYSTEM_ERROR;
        }
        if (str == "PROCESS_INTERRUPTED") {
            return StatusType::PROCESS_INTERRUPTED;
        }
        return StatusType::UNKNOWN;
    }

    string status_type_to_string(StatusType status_type) {
        switch (status_type) {
            case StatusType::IN_PROGRESS:
                return "IN_PROGRESS";
            case StatusType::FINISH:
                return "FINISH";
            case StatusType::EXPIRE:
                return "EXPIRE";
            case StatusType::PAUSED:
                return "PAUSED";
            case StatusType::STOP:
                return "STOP";
            case StatusType::WARNING:
                return "WARNING";
            case StatusType::DATA_ERROR:
                return "DATA_ERROR";
            case StatusType::TASK_ERROR:
                return "TASK_ERROR";
            case StatusType::SYSTEM_ERROR:
                return "SYSTEM_ERROR";
            case StatusType::PROCESS_INTERRUPTED:
                return "PROCESS_INTERRUPTED";
            default:
                return "UNKNOWN";
        }
    }
}
//
// Created by Rong Zhou on 2018-07-30.
//

#ifndef ATS_TASKREPORT_H
#define ATS_TASKREPORT_H

#include <string>
#include <vector>
#include <chrono>
#include "task.h"
#include "types/statustype.h"
#include "types/sidetype.h"

namespace ats {

    struct TaskReport {
        static std::vector<std::string> to_record(const TaskReport &task_report);

        static nlohmann::json to_json(const TaskReport &report);

        static std::vector<std::pair<std::string, std::string>> to_vector(const TaskReport &report);

        TaskReport(uint32_t task_id, Type task_type, size_t unique_id);

        uint32_t task_id;
        Type task_type;
        size_t unique_id;
        std::string ticker;
        int64_t quantity;
        double amount;
        SideType side;
        std::chrono::system_clock::time_point start_time;
        std::chrono::system_clock::time_point end_time;
        std::string status;
        double progress;
        uint64_t order_num;
        uint64_t trade_num;
        double avg_traded_price;
        double avg_market_price;
        int64_t traded_quantity;
        int64_t pending_quantity;
        double traded_amount;
        double pending_amount;
        double predicted_amount;
        double left_amount;
        int64_t left_quantity;
        double bps;
        double passive_ratio;
        std::string remark;
    };

}


#endif //ATS_TASKREPORT_H

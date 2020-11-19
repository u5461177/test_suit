//
// Created by Rong Zhou on 2018-07-30.
//

#ifndef ATS_MINUTEREPORT_H
#define ATS_MINUTEREPORT_H

#include <string>
#include <vector>
#include "task.h"

namespace ats {

    struct MinuteReport {
        static std::vector<std::string> to_record(const MinuteReport &minute_report);

        static nlohmann::json to_json(const MinuteReport &report);

        MinuteReport(uint32_t task_id, Type task_type, size_t unique_id);

        uint32_t task_id;
        Type task_type;
        size_t unique_id;
        double market_price;
        double avg_market_price;
        double avg_traded_price;
        int64_t task_traded_quantity;
        int64_t market_traded_quantity;
        double trade_ratio;
        std::string trade_time;
    };

}

#endif //ATS_MINUTEREPORT_H

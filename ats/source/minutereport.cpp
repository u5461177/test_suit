//
// Created by Rong Zhou on 2018-07-30.
//

#include "minutereport.h"
#include <json.hpp>

namespace ats {

    using namespace std;
    using json = nlohmann::json;

    vector<string> MinuteReport::to_record(const MinuteReport &minute_report) {
        static const string report_type_("report_type");
        static const string task_type_("task_type");
        static const string task_id_("task_id");
        static const string unique_id_("unique_id");
        static const string market_price_("market_price");
        static const string avg_market_price_("avg_market_price");
        static const string avg_traded_price_("avg_traded_price");
        static const string task_traded_quantity_("task_traded_quantity");
        static const string market_traded_quantity_("market_traded_quantity");
        static const string trade_ratio_("trade_ratio");
        static const string trade_time_("trade_time");
        auto report_type = "minute_report";
        auto task_id = minute_report.task_id;
        auto unique_id = to_string(minute_report.unique_id);
        auto market_price = to_string(minute_report.market_price);
        auto avg_market_price = to_string(minute_report.avg_market_price);
        auto avg_traded_price = to_string(minute_report.avg_traded_price);
        auto task_traded_quantity = to_string(minute_report.task_traded_quantity);
        auto market_traded_quantity = to_string(minute_report.market_traded_quantity);
        auto trade_ratio = to_string(minute_report.trade_ratio);
        auto trade_time = minute_report.trade_time;
        return vector<string>{report_type_, report_type, task_id_, to_string(task_id), unique_id_,
                              unique_id, market_price_, market_price, avg_market_price_, avg_market_price,
                              avg_traded_price_, avg_traded_price, task_traded_quantity_, task_traded_quantity,
                              market_traded_quantity_, market_traded_quantity,
                              trade_ratio_, trade_ratio, trade_time_, trade_time};
    }

    json MinuteReport::to_json(const MinuteReport &report) {
        json result;
        result["report_type"] = "minute_report";
        result["task_type"] = Task::type_to_string(report.task_type);
        result["task_id"] = report.task_id;
        result["unique_id"] = report.unique_id;
        result["market_price"] = report.market_price;
        result["avg_market_price"] = report.avg_market_price;
        result["avg_traded_price"] = report.avg_traded_price;
        result["task_traded_quantity"] = report.task_traded_quantity;
        result["market_traded_quantity"] = report.market_traded_quantity;
        result["trade_ratio"] = report.trade_ratio;
        result["trade_time"] = report.trade_time;
        return result;
    }

    MinuteReport::MinuteReport(uint32_t task_id, Type task_type, size_t unique_id) {
        this->task_id = task_id;
        this->task_type = task_type;
        this->unique_id = unique_id;
    }
}
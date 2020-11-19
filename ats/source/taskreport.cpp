//
// Created by Rong Zhou on 2018-07-30.
//

#include "taskreport.h"
#include <json.hpp>
#include "util/timehelper.h"

namespace ats {

    using namespace std;
    using json = nlohmann::json;

    vector<std::string> TaskReport::to_record(const TaskReport &task_report) {
        static const string report_type_("report_type");
        static const string task_type_("task_type");
        static const string task_id_("task_id");
        static const string unique_id_("unique_id");
        static const string ticker_("ticker");
        static const string quantity_("quantity");
        static const string amount_("amount");
        static const string side_("side");
        static const string start_time_("start_time");
        static const string end_time_("end_time");
        static const string status_("status");
        static const string progress_("progress");
        static const string order_num_("order_num");
        static const string trade_num_("trade_num");
        static const string avg_traded_price_("avg_traded_price");
        static const string avg_market_price_("avg_market_price");
        static const string traded_quantity_("traded_quantity");
        static const string pending_quantity_("pending_quantity");
        static const string traded_amount_("traded_amount");
        static const string pending_amount_("pending_amount");
        static const string predicted_amount_("predicted_amount");
        static const string left_amount_("left_amount");
        static const string left_quantity_("left_quantity");
        static const string bps_("bps");
        static const string passive_ratio_("passive_ratio");
        static const string remark_("remark");
        auto report_type = "task_report";
        auto task_id = to_string(task_report.task_id);
        auto task_type = Task::type_to_string(task_report.task_type);
        auto unique_id = to_string(task_report.unique_id);
        auto ticker = task_report.ticker;
        auto quantity = to_string(task_report.quantity);
        auto amount = to_string(task_report.amount);
        auto side = side_type_to_string(task_report.side);
        auto start_time = time_point_to_string(task_report.start_time);
        auto end_time = time_point_to_string(task_report.end_time);
        auto status = task_report.status;
        auto progress = to_string(task_report.progress);
        auto order_num = to_string(task_report.order_num);
        auto trade_num = to_string(task_report.trade_num);
        auto avg_traded_price = to_string(task_report.avg_traded_price);
        auto avg_market_price = to_string(task_report.avg_market_price);
        auto traded_quantity = to_string(task_report.traded_quantity);
        auto pending_quantity = to_string(task_report.pending_quantity);
        auto traded_amount = to_string(task_report.traded_amount);
        auto pending_amount = to_string(task_report.pending_amount);
        auto predicted_amount = to_string(task_report.predicted_amount);
        auto left_amount = to_string(task_report.left_amount);
        auto left_quantity = to_string(task_report.left_quantity);
        auto bps = to_string(task_report.bps);
        auto passive_ratio = to_string(task_report.passive_ratio);
        auto remark = task_report.remark;
        return vector<string>{report_type_, report_type, task_id_, task_id, task_type_, task_type, unique_id_,
                              unique_id, ticker_, ticker, quantity_, quantity, amount_, amount, side_, side, status_,
                              status, progress_, progress, order_num_, order_num, trade_num_, trade_num,
                              avg_traded_price_, avg_traded_price, avg_market_price_, avg_market_price,
                              traded_quantity_, traded_quantity, traded_amount_, traded_amount, pending_amount_,
                              pending_amount, bps_, bps, passive_ratio_, passive_ratio, remark_, remark,
                              left_amount_, left_amount, left_quantity_, left_quantity,
                              start_time_, start_time, end_time_, end_time};
    }

    json TaskReport::to_json(const TaskReport &report) {
        json result;
        result["report_type"] = "task_report";
        result["task_id"] = report.task_id;
        result["task_type"] = Task::type_to_string(report.task_type);
        result["unique_id"] = report.unique_id;
        result["start_time"] = time_point_to_string(report.start_time);
        result["end_time"] = time_point_to_string(report.end_time);
        result["ticker"] = report.ticker;
        result["quantity"] = report.quantity;
        result["amount"] = report.amount;
        result["side"] = side_type_to_string(report.side);
        result["status"] = report.status;
        result["progress"] = report.progress;
        result["order_num"] = report.order_num;
        result["trade_num"] = report.trade_num;
        result["avg_traded_price"] = report.avg_traded_price;
        result["avg_market_price"] = report.avg_market_price;
        result["traded_quantity"] = report.traded_quantity;
        result["pending_quantity"] = report.pending_quantity;
        result["traded_amount"] = report.traded_amount;
        result["pending_amount"] = report.pending_amount;
        result["predicted_amount"] = report.predicted_amount;
        result["left_amount"] = report.left_amount;
        result["left_quantity"] = report.left_quantity;
        result["bps"] = report.bps;
        result["passive_ratio"] = report.passive_ratio;
        result["remark"] = report.remark;
        return result;
    }

    vector<pair<string, string>> TaskReport::to_vector(const TaskReport &report) {
        vector<pair<string, string>> result;
        result.emplace_back(make_pair("report_type", "task_report"));
        result.emplace_back(make_pair("report_type", "task_report"));
        result.emplace_back(make_pair("task_id", to_string(report.task_id)));
        result.emplace_back(make_pair("task_type", Task::type_to_string(report.task_type)));
        result.emplace_back(make_pair("unique_id", to_string(report.unique_id)));
        result.emplace_back(make_pair("start_time", time_point_to_string(report.start_time)));
        result.emplace_back(make_pair("end_time", time_point_to_string(report.end_time)));
        result.emplace_back(make_pair("ticker", report.ticker));
        result.emplace_back(make_pair("quantity", to_string(report.quantity)));
        result.emplace_back(make_pair("amount", to_string(report.amount)));
        result.emplace_back(make_pair("side", side_type_to_string(report.side)));
        result.emplace_back(make_pair("status", report.status));
        result.emplace_back(make_pair("progress", to_string(report.progress)));
        result.emplace_back(make_pair("order_num", to_string(report.order_num)));
        result.emplace_back(make_pair("trade_num", to_string(report.trade_num)));
        result.emplace_back(make_pair("avg_traded_price", to_string(report.avg_traded_price)));
        result.emplace_back(make_pair("avg_market_price", to_string(report.avg_market_price)));
        result.emplace_back(make_pair("traded_quantity", to_string(report.traded_quantity)));
        result.emplace_back(make_pair("pending_quantity", to_string(report.pending_quantity)));
        result.emplace_back(make_pair("traded_amount", to_string(report.traded_amount)));
        result.emplace_back(make_pair("pending_amount", to_string(report.pending_amount)));
        result.emplace_back(make_pair("predicted_amount", to_string(report.predicted_amount)));
        result.emplace_back(make_pair("left_amount", to_string(report.left_amount)));
        result.emplace_back(make_pair("left_quantity", to_string(report.left_quantity)));
        result.emplace_back(make_pair("bps", to_string(report.bps)));
        result.emplace_back(make_pair("passive_ratio", to_string(report.passive_ratio)));
        result.emplace_back(make_pair("remark", report.remark));
        return result;
    }

    TaskReport::TaskReport(uint32_t task_id, Type task_type, size_t unique_id) {
        this->task_id = task_id;
        this->task_type = task_type;
        this->unique_id = unique_id;
    }

}
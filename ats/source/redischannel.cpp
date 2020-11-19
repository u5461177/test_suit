//
// Created by Rong Zhou on 2018-08-03.
//

#include "redischannel.h"
#include "redisserver.h"
#include "glog/logging.h"

namespace ats {

    using namespace std;
    using namespace std::chrono;
    using json = nlohmann::json;
    using time_point = std::chrono::system_clock::time_point;

    RedisChannel::RedisChannel(shared_ptr<User> owner) : Channel(move(owner), "") {
    }

    void RedisChannel::send_user_relogin(const std::string &username) {
        //TODO
    }

    void
    RedisChannel::send_task_status(const string &username, Type task_type, uint32_t task_id,
                                   shared_ptr<TaskReport> task_report, const string &user_mac) {
        auto record = TaskReport::to_record(*task_report);
        vector<string> datetime{"report_date", date_to_string(system_clock::now()), "report_time",
                                time_point_to_string(system_clock::now()), "username", username,
                                "decryptedMac",user_mac,
                                "task_type", Task::type_to_string(task_type)};
        record.insert(record.end(), make_move_iterator(datetime.begin()), make_move_iterator(datetime.end()));
        RedisServer::instance().on_task_report_changed(username, task_type, task_id, task_report, user_mac);
        RedisServer::instance().write_stream("task_status", record);
    }


    void RedisChannel::send_order_info(const string &username, Type task_type, uint32_t task_id,
                                       shared_ptr<OrderInfo> order_info,
                                       shared_ptr<ErrorInfo> error_info,
                                       size_t order_uid, const pair<ats::PriceAction,
            shared_ptr<MarketQuotationData>> market_quotation_data) {
        auto record = OrderInfo::to_record(*order_info);
        auto error = ErrorInfo::to_json(*error_info);

        vector<string> datetime{"report_date", date_to_string(system_clock::now()), "report_time",
                                time_point_to_string(system_clock::now()), "username", username, "task_type",
                                Task::type_to_string(task_type), "task_id", to_string(task_id), "unique_id",
                                to_string(order_uid), "error_id",
                                to_string(error["error_id"].get<int32_t>()), "error_msg",
                                error["error_msg"].get<string>(),
                                "price_action", price_action_to_string(market_quotation_data.first),
                                "bid1", to_string(market_quotation_data.second->_bid[0]),
                                "bid_qty1", to_string(market_quotation_data.second->_bid_qty[0]),
                                "bid2", to_string(market_quotation_data.second->_bid[1]),
                                "bid_qty2", to_string(market_quotation_data.second->_bid_qty[1]),
                                "bid3", to_string(market_quotation_data.second->_bid[2]),
                                "bid_qty3", to_string(market_quotation_data.second->_bid_qty[2]),
                                "bid4", to_string(market_quotation_data.second->_bid[3]),
                                "bid_qty4", to_string(market_quotation_data.second->_bid_qty[3]),
                                "bid5", to_string(market_quotation_data.second->_bid[5]),
                                "bid_qty5", to_string(market_quotation_data.second->_bid_qty[5]),
                                "ask1", to_string(market_quotation_data.second->_ask[0]),
                                "ask_qty1", to_string(market_quotation_data.second->_ask_qty[0]),
                                "ask2", to_string(market_quotation_data.second->_ask[1]),
                                "ask_qty2", to_string(market_quotation_data.second->_ask_qty[1]),
                                "ask3", to_string(market_quotation_data.second->_ask[2]),
                                "ask_qty3", to_string(market_quotation_data.second->_ask_qty[2]),
                                "ask4", to_string(market_quotation_data.second->_ask[3]),
                                "ask_qty4", to_string(market_quotation_data.second->_ask_qty[3]),
                                "ask5", to_string(market_quotation_data.second->_ask[5]),
                                "ask5_qty5", to_string(market_quotation_data.second->_ask_qty[5])};
        record.insert(record.end(), make_move_iterator(datetime.begin()), make_move_iterator(datetime.end()));
        auto stream_name = "order_info." + to_string(task_id);
        RedisServer::instance().write_stream(stream_name, record);
    }

    void
    RedisChannel::send_order_cancel_info(const string &username, Type task_type, uint32_t task_id,
                                         shared_ptr<OrderCancelInfo> order_info,
                                         shared_ptr<ErrorInfo> error_info) {
        //TODO send message in redis
    }

    void
    RedisChannel::send_trade_report(const string &username, Type task_type, uint32_t task_id,
                                    shared_ptr<TradeReport> trade_report,
                                    size_t trade_uid) {
        auto record = TradeReport::to_record(*trade_report);
        vector<string> datetime{"report_date", date_to_string(system_clock::now()), "report_time",
                                time_point_to_string(system_clock::now()), "username", username, "task_type",
                                Task::type_to_string(task_type), "task_id", to_string(task_id), "unique_id",
                                to_string(trade_uid)};
        record.insert(record.end(), make_move_iterator(datetime.begin()), make_move_iterator(datetime.end()));
        auto stream_name = "trade_report." + to_string(task_id);
        RedisServer::instance().write_stream(stream_name, record);
    }

    void RedisChannel::send_minute_report(const string &username, Type task_type, uint32_t task_id,
                                          shared_ptr<MinuteReport> minute_report) {
        auto record = MinuteReport::to_record(*minute_report);
        vector<string> datetime{"report_date", date_to_string(system_clock::now()), "report_time",
                                time_point_to_string(system_clock::now()), "username", username, "task_type",
                                Task::type_to_string(task_type)};
        record.insert(record.end(), make_move_iterator(datetime.begin()), make_move_iterator(datetime.end()));
        auto stream_name = "minute_report." + to_string(task_id);
        RedisServer::instance().write_stream(stream_name, record);
    }

    vector<string> RedisChannel::split(const string &str, char delim, vector<string> &elems, bool skip_empty) {
        istringstream iss(str);
        for (string item; getline(iss, item, delim);)
            if (skip_empty && item.empty()) continue;
            else elems.push_back(item);
        return elems;
    }

    vector<uint32_t> RedisChannel::read_task_id(const string &username) {
        auto set_name = username + ".tasks";
        auto reply = RedisServer::instance().read_sets(set_name);
        vector<uint32_t> task_ids;
        auto reply_res = reply.get().as_array();
        for (auto &res : reply_res) {
            vector<string> result;
            split(res.as_string(), ',', result);
            if (result[1] == date_to_string(system_clock::now())) {
                task_ids.push_back((uint32_t) stoi(result[0]));
            }
            result.clear();
        }
        sort(task_ids.rbegin(), task_ids.rend());
        return task_ids;
    }

    uint32_t RedisChannel::read_last_task_id() {
        auto reply = RedisServer::instance().read_sets("tasks");
        vector<uint32_t> task_ids{};
        auto reply_res = reply.get().as_array();
        if (reply_res.empty())
            return 0;
        for (auto &res : reply_res) {
            task_ids.push_back((uint32_t) stoi(res.as_string()));
        }
        auto biggest = max_element(std::begin(task_ids), std::end(task_ids));
        return *biggest;
    }

    vector<string>
    RedisChannel::read_minute_report(const string &stream, const string &username, uint32_t task_id, size_t count,
                                     uint32_t start_id, uint32_t end_id) {
        auto reply = RedisServer::instance().read_stream(stream, username, task_id, count, start_id, end_id);
        vector<string> reports;
        auto reply_res = reply.get();
        for (auto &val : reply_res.as_array()) {
            for (auto &res : val.as_array()) {
                if (res.is_array()) {
                    json response;
                    auto result = res.as_array();
                    for (size_t i = 0; i < result.size();) {
                        response[result.at(i).as_string()] = result.at(i + 1).as_string();
                        i += 2;
                    }
                    if (stoul(response["unique_id"].get<string>()) >= start_id
                        && response["task_id"].get<string>() == to_string(task_id)
                        && response["report_date"].get<string>() == date_to_string(system_clock::now())
                        && response["username"].get<string>() == username) {
                        reports.push_back(response.dump());
                    }
                }
            }
        }
        return reports;
    }

    vector<string>
    RedisChannel::read_history_report(const string &stream, const string &username, uint32_t task_id, size_t count,
                                      uint32_t start_id, uint32_t end_id) {
        auto reply = RedisServer::instance().read_stream(stream, username, task_id, count, start_id, end_id);
        vector<string> reports;
        auto reply_res = reply.get();
        for (auto &val : reply_res.as_array()) {
            for (auto &res : val.as_array()) {
                if (res.is_array()) {
                    json response;
                    auto result = res.as_array();
                    for (size_t i = 0; i < result.size();) {
                        response[result.at(i).as_string()] = result.at(i + 1).as_string();
                        i += 2;
                    }
                    if (stoul(response["unique_id"].get<string>()) >= start_id
                        && response["task_id"].get<string>() == to_string(task_id)
                        && response["report_date"].get<string>() == date_to_string(system_clock::now())
                        && response["username"].get<string>() == username
                            ) {
                        reports.push_back(response.dump());
                    }
                }
            }
        }
        return reports;
    }

    vector<string> RedisChannel::read_last_history_report(const string &hash, const string &username) {
        auto task_ids = read_task_id(username);
        auto tasks_size = task_ids.size();
        vector<string> reports;
        unsigned int id = 0;
        for (auto &task_ele : task_ids) {
            json response;
            string report_str;
            if (hash == "order" || hash == "trade" || hash == "task") {
                auto hash_name = hash + "." + to_string(task_ele);
                auto reply = RedisServer::instance().read_hashes(hash_name);
                auto reply_res = reply.get().as_array();
                for (size_t i = 0; i < reply_res.size();) {
                    response[reply_res.at(i).as_string()] = reply_res.at(i + 1).as_string();
                    i += 2;
                }
                if (tasks_size - 1 == id) {
                    response["is_last"] = "true";
                }
                report_str = response.dump();
            } else {
                LOG(ERROR) << "No such history reports: " << hash;
            }
            id++;
            reports.push_back(report_str);
        }
        return reports;
    }
}
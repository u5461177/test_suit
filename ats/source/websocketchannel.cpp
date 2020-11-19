//
// Created by Rong Zhou on 2018-08-03.
//

#include "websocketchannel.h"
#include <json.hpp>
#include "user.h"
#include "glog/logging.h"
#include "usermanager.h"
#include "restserver.h"

namespace ats {

    using namespace std;
    using namespace restbed;
    using json = nlohmann::json;
    using namespace std::chrono;
    using time_point = std::chrono::system_clock::time_point;

    WebSocketChannel::WebSocketChannel(shared_ptr<User> owner, shared_ptr<WebSocket> socket)
            : Channel(move(owner), socket->get_key()), _websocket(socket) {
        socket->set_close_handler([this](shared_ptr<WebSocket> socket) {
            on_socket_close(socket);
        });
        socket->set_error_handler([this](shared_ptr<WebSocket> socket, error_code error) {
            on_socket_error(socket, error);
        });
        socket->set_message_handler([this](shared_ptr<WebSocket> socket, shared_ptr<WebSocketMessage> message) {
            on_socket_message(socket, message);
        });
        socket->send("welcome");
        time_t now = time(nullptr);
        tm t = *localtime(&now);
        t.tm_hour = 1;
        t.tm_min = 30;
        t.tm_sec = 0;
        _open_time = system_clock::from_time_t(mktime(&t));
        _update_order_time = std::chrono::system_clock::now();
        _update_trade_time = std::chrono::system_clock::now();
        _update_task_time = std::chrono::system_clock::now();
        _update_minute_time = std::chrono::system_clock::now();
    }

    WebSocketChannel::~WebSocketChannel() {
        LOG(INFO) << "websocket channel will be disconnected";
        if (_websocket->is_open()) {
            _websocket->close();
        }
//        RESTServer::instance().on_websocket_disconnected(_websocket);

    }


    string WebSocketChannel::vector_to_string(const std::vector<std::string> &report) {
        if (report.empty()) {
            return "[]";
        }
        stringstream report_ss;
        bool first = true;
        for (auto &element : report) {
            if (!first) {
                report_ss << ", ";
            }
            if (first) {
                report_ss << "[";
            }
            report_ss << element;
            first = false;
        }
        report_ss << "]";
        return report_ss.str();
    }

    void WebSocketChannel::send_user_relogin(const std::string &username) {
        LOG(INFO) << "relogin msg send to " << username;
        json relogin_data;
        relogin_data["report_date"] = date_to_string(system_clock::now());
        relogin_data["report_time"] = time_point_to_string(system_clock::now());
        relogin_data["username"] = username;
        relogin_data["report_type"] = "relogin_report";
        _relogin_data.push_back(relogin_data.dump());
        _update_task_time = system_clock::now();
        auto task_msg = vector_to_string(_relogin_data);
        _websocket->send(task_msg);
        _relogin_data.clear();

    }

    void WebSocketChannel::send_task_id(const std::string &username, uint32_t task_id, uint64_t task_user_id){
        LOG(INFO) << "task_id send to " << username << "| task id: "<< task_id;
        json task_id_json;
        task_id_json["username"] = username;
        task_id_json["report_type"] = "task_id_report";
        task_id_json["task_id"] = task_id;
        task_id_json["id"] = task_user_id;
        _task_id_data.push_back(task_id_json.dump());
        _update_task_time = system_clock::now();
        auto task_msg = vector_to_string(_task_id_data);
        _websocket->send(task_msg);
        _task_id_data.clear();

    }


    void WebSocketChannel::send_task_status(const string &username, Type task_type, uint32_t task_id,
                    shared_ptr<TaskReport> task_status, const string &user_mac) {
        if (_task_report_subscribed) {
            LOG(INFO) << "task report send to: " << username << " | task id: " << task_id;
            auto data = TaskReport::to_json(*task_status);
            data["report_date"] = date_to_string(system_clock::now());
            data["report_time"] = time_point_to_string(system_clock::now());
            data["username"] = username;
            _task_data.push_back(data.dump());

            _update_task_time = system_clock::now();
            auto task_msg = vector_to_string(_task_data);
            _websocket->send(task_msg);
            _task_data.clear();
        }
    }

    void WebSocketChannel::send_order_info(const string &username, Type task_type, uint32_t task_id,
                                           shared_ptr<OrderInfo> order_info,
                                           shared_ptr<ErrorInfo> error_info,
                                           size_t order_uid, const pair<PriceAction,
            shared_ptr<MarketQuotationData>> market_quotation_data) {
        if (_order_report_subscribed) {
            LOG(INFO) << "order report send to: " << username << " | task id: " << task_id;
            auto data = OrderInfo::to_json(*order_info);
            auto error = ErrorInfo::to_json(*error_info);
            data["task_type"] = Task::type_to_string(task_type);
            data["task_id"] = to_string(task_id);
            data["unique_id"] = to_string(order_uid);
            data["error_id"] = error["error_id"];
            data["error_msg"] = error["error_msg"];
            data["report_date"] = date_to_string(system_clock::now());
            data["report_time"] = time_point_to_string(system_clock::now());
            data["username"] = username;
            _order_data.push_back(data.dump());
            _update_order_time = system_clock::now();
            auto order_msg = vector_to_string(_order_data);
            _websocket->send(order_msg);
            _order_data.clear();
        }
    }
    void WebSocketChannel::send_order_cancel_info(const string &username, Type task_type, uint32_t task_id,
                                                  shared_ptr<OrderCancelInfo> order_info,
                                                  std::shared_ptr<ErrorInfo> error_info) {
        //TODO send message to client ?

    }

    void
    WebSocketChannel::send_trade_report(const string &username, Type task_type, uint32_t task_id,
                                        shared_ptr<TradeReport> trade_report, size_t trade_uid) {
        if (_trade_report_subscribed) {
            LOG(INFO) << "trade report send to: " << username << " | task id: " << task_id;;
            auto data = TradeReport::to_json(*trade_report);
            data["task_type"] = Task::type_to_string(task_type);
            data["task_id"] = to_string(task_id);
            data["unique_id"] = to_string(trade_uid);
            data["report_date"] = date_to_string(system_clock::now());
            data["report_time"] = time_point_to_string(system_clock::now());
            data["username"] = username;
            _trade_data.push_back(data.dump());
            _update_trade_time = system_clock::now();
            auto trade_msg = vector_to_string(_trade_data);
            _websocket->send(trade_msg);
            _trade_data.clear();
        }
    }

    void WebSocketChannel::send_minute_report(const string &username, Type task_type, uint32_t task_id,
                                              shared_ptr<MinuteReport> minute_report) {
        if (_minute_report_subscribed) {
            LOG(INFO) << "minute report send to: " << username << " | task id: " << task_id;
            auto data = MinuteReport::to_json(*minute_report);
            data["report_date"] = date_to_string(system_clock::now());
            data["report_time"] = time_point_to_string(system_clock::now());
            data["username"] = username;
            _minute_data.push_back(data.dump());
            _update_minute_time = system_clock::now();
            auto minute_msg = vector_to_string(_minute_data);
            _websocket->send(minute_msg);
            _minute_data.clear();
        }
    }

    uint32_t WebSocketChannel::read_last_task_id() {
        return 0;
    }

    vector<string>
    WebSocketChannel::read_minute_report(const string &stream, const string &username, uint32_t task_id, size_t count,
                                         uint32_t start_id, uint32_t end_id) {
        return vector<string>{};
    }

    vector<string>
    WebSocketChannel::read_history_report(const string &stream, const string &username, uint32_t task_id, size_t count,
                                          uint32_t start_id, uint32_t end_id) {
        return vector<string>{};
    }

    vector<string> WebSocketChannel::read_last_history_report(const string &hash, const string &username) {
        return vector<string>{};
    };

    void WebSocketChannel::on_socket_close(shared_ptr<WebSocket> socket) {
        if (socket->is_open()) {
            auto opcode = WebSocketMessage::CONNECTION_CLOSE_FRAME;
            auto response = make_shared<WebSocketMessage>(opcode, Bytes({10, 00}));
            socket->send(response);
        }
        if (auto owner = _owner.lock()) {
            owner->remove_channel(_channel_id);
        }
    }

    void WebSocketChannel::on_socket_error(shared_ptr<WebSocket> socket, error_code error) {
        auto error_msg = error.message();
        LOG(WARNING) << "websocket error: " << error.message();
    }

    void WebSocketChannel::_subscribed_result_report(const string &report_type) {
        json subscribed_result = {};
        subscribed_result["report_type"] = "subscribe_result";
        subscribed_result["subscribe_type"] = report_type;
        subscribed_result["msg"] = "Success";
        subscribed_result["code"] = "0";
        _websocket->send(subscribed_result.dump());
    }

    void WebSocketChannel::_unsubscribed_result_report(const string &report_type) {
        json subscribed_result = {};
        subscribed_result["report_type"] = "unsubscribe_result";
        subscribed_result["unsubscribe_type"] = report_type;
        subscribed_result["msg"] = "Success";
        subscribed_result["code"] = "0";
        _websocket->send(subscribed_result.dump());
    }

    void WebSocketChannel::on_socket_message(shared_ptr<WebSocket> socket, shared_ptr<WebSocketMessage> message) {
        auto opcode = message->get_opcode();
        if (opcode == WebSocketMessage::CONNECTION_CLOSE_FRAME) {
            socket->close();
        } else if (opcode == WebSocketMessage::PING_FRAME) {
            opcode = WebSocketMessage::PONG_FRAME;
            auto response = make_shared<WebSocketMessage>(opcode, message->get_data());
            socket->send(response);
        } else if (opcode == WebSocketMessage::PONG_FRAME) {
            LOG(INFO) << "receive websocket pong";
        } else if (opcode == WebSocketMessage::BINARY_FRAME) {
            //We don't support binary data.
            opcode = WebSocketMessage::CONNECTION_CLOSE_FRAME;
            auto response = make_shared<WebSocketMessage>(opcode, Bytes({10, 03}));
            socket->send(response);
        } else if (opcode == WebSocketMessage::TEXT_FRAME) {
            auto data = message->get_data();
            try {
                auto json_data = json::parse(data);
                auto command_str = json_data["command"].get<string>();
                auto command = string_to_command(command_str);
                switch (command) {
                    case Command::SUBSCRIBE_TASK_REPORT: {
                        _task_report_subscribed = true;
                        _subscribed_result_report("subscribe_task_report_result");
                        break;
                    }
                    case Command::UNSUBSCRIBE_TASK_REPORT:
                        _task_report_subscribed = false;
                        _unsubscribed_result_report("unsubscribe_task_report_result");
                        break;
                    case Command::SUBSCRIBE_ORDER_REPORT:
                        _order_report_subscribed = true;
                        _subscribed_result_report("subscribe_order_report_result");
                        break;
                    case Command::UNSUBSCRIBE_ORDER_REPORT:
                        _order_report_subscribed = false;
                        _unsubscribed_result_report("unsubscribe_order_report_result");
                        break;
                    case Command::SUBSCRIBE_TRADE_REPORT:
                        _trade_report_subscribed = true;
                        _subscribed_result_report("subscribe_trade_report_result");
                        break;
                    case Command::UNSUBSCRIBE_TRADE_REPORT:
                        _trade_report_subscribed = false;
                        _unsubscribed_result_report("unsubscribe_trade_report_result");
                        break;
                    case Command::SUBSCRIBE_MINUTE_REPORT: {
                        _minute_report_subscribed = true;
                        try {
                            _subscribed_task_id = json_data["task_id"].get<uint32_t>();
                        } catch (const json::exception &e) {
                            _subscribed_task_id = 0;
                        }
                        _subscribed_result_report("subscribe_minute_report_result");
                        break;
                    }
                    case Command::UNSUBSCRIBE_MINUTE_REPORT:
                        _minute_report_subscribed = false;
                        _unsubscribed_result_report("unsubscribe_minute_report_result");
                        break;
                    case Command::GET_TASK_REPORT: {
                        auto username = json_data["username"].get<string>();
                        auto user = UserManager::instance().find_or_create_user(username);
                        LOG(INFO) << "get task report send to: " << username;
                        auto result = user->find_channel("")->read_last_history_report("task", username);
                        if (result.empty()) {
                            json w;
                            w["report_type"] = "task_report";
                            result.emplace_back(w.dump());
                        }
                        _websocket->send(vector_to_string(result));
                        break;
                    }
                    case Command::GET_ORDER_REPORT: {
                        auto username = json_data["username"].get<string>();
                        auto user = UserManager::instance().find_or_create_user(username);
                        LOG(INFO) << "get order report send to: " << username;
                        uint32_t task_id = 0;
                        try {
                            task_id = json_data["task_id"].get<uint32_t>();
                        } catch (const json::exception &e) {
                            LOG(WARNING) << "task id was not found";
                        }
                        auto stream_name = "order_info." + to_string(task_id);
                        auto result = user->find_channel("")->read_history_report(stream_name, username, task_id, 0, 0,
                                                                                  0);
                        if (result.empty()) {
                            json w;
                            w["report_type"] = "order_report";
                            result.emplace_back(w.dump());
                        }
                        _websocket->send(vector_to_string(result));
                        break;
                    }
                    case Command::GET_TRADE_REPORT: {
                        auto username = json_data["username"].get<string>();
                        auto user = UserManager::instance().find_or_create_user(username);
                        uint32_t task_id = 0;
                        try {
                            task_id = json_data["task_id"].get<uint32_t>();
                        } catch (const json::exception &e) {
                            LOG(WARNING) << "task id was not found";
                        }
                        LOG(INFO) << "get trade report send to: " << username;
                        auto stream_name = "trade_report." + to_string(task_id);
                        auto result = user->find_channel("")->read_history_report(stream_name, username, task_id, 0, 0,
                                                                                  0);
                        if (result.empty()) {
                            json w;
                            w["report_type"] = "trade_report";
                            result.emplace_back(w.dump());
                        }
                        _websocket->send(vector_to_string(result));
                        break;
                    }
                    case Command::GET_MINUTE_REPORT: {
                        try {
                            auto username = json_data["username"].get<string>();
                            uint32_t task_id = 0;
                            try {
                                task_id = json_data["task_id"].get<uint32_t>();
                            } catch (const json::exception &e) {
                                LOG(WARNING) << "task id was not found";
                            }
                            auto user = UserManager::instance().find_or_create_user(username);
                            LOG(INFO) << "get minute report send to: " << username << " | task id: " << task_id;
                            auto stream_name = "minute_report." + to_string(task_id);
                            auto result = user->find_channel("")->read_minute_report(stream_name, username,
                                                                                     task_id, 0, 0, 0);
                            if (result.empty()) {
                                json w;
                                w["report_type"] = "minute_report";
                                result.emplace_back(w.dump());
                            }
                            _websocket->send(vector_to_string(result));
                            break;
                        } catch (const json::exception &e) {
                            _websocket->send("command error");
                        }
                    }
                    default:
                        break;
                    case Command::UNKNOWN:
                        break;
                }
            } catch (const json::exception &e) {
                LOG(ERROR) << "command parse error";
            }
        }
    }

    system_clock::duration WebSocketChannel::calc_time_span(system_clock::time_point tp) const {
        //  TODO: For open test
        return tp - _open_time;
    }

}
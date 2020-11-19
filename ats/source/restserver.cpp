//
// Created by rongzhou on 18-7-29.
//

#include "restserver.h"
#include "configuration.h"
#include "redisserver.h"
#include "usermanager.h"
#include "websocketchannel.h"
#include "util/cryptohelper.h"
#include "types/errorcode.h"
#include "util/timehelper.h"
#include "taskfactory.h"
#include "glog/logging.h"
#include "repocapitalcalculator.h"
#include "cashcapitalcalculator.h"
#include "taskinputdata.h"
#include <openssl/sha.h>

namespace ats {
    using namespace std;
    using time_point = std::chrono::system_clock::time_point;
    using namespace restbed;
    using json = nlohmann::json;

    RESTServer::~RESTServer() {
        for (auto &i : _websockets) {
            auto &socket = *i;
            socket.close();
        }
    }

    void RESTServer::start() {
        _max_tasks_size  = Configuration::instance().get_tasks_limit_num();

        const multimap<string, string> filters{
                {"Accept",       "application/json"},
                {"Content-Type", "application/json"}
        };
        auto resource = make_shared<Resource>();
        resource->set_path("login");
        resource->set_method_handler("POST", filters, [this](shared_ptr<Session> session) {
            on_user_login(session);
        });
        resource->set_failed_filter_validation_handler([this](shared_ptr<Session> session) {
            on_filter_validation_failed(session);
        });
        resource->set_error_handler([this](const int, const exception &e, const shared_ptr<Session> session){
            on_resource_error_handler(0,e,session);

        });
        _rest_service.publish(resource);


        resource = make_shared<Resource>();
        resource->set_path("logout");
        resource->set_method_handler("POST", filters, [this](shared_ptr<Session> session) {
            on_user_logout(session);
        });
        resource->set_failed_filter_validation_handler([this](shared_ptr<Session> session) {
            on_filter_validation_failed(session);
        });
        resource->set_error_handler([this](const int,const exception &e, const shared_ptr<Session> session){
            on_resource_error_handler(0,e,session);

        });
        _rest_service.publish(resource);


        resource = make_shared<Resource>();
        resource->set_paths({"start_task", "start_amount_task"});
        resource->set_method_handler("POST", filters, [this](shared_ptr<Session> session) {
            on_task_start(session);
        });
        resource->set_failed_filter_validation_handler([this](shared_ptr<Session> session) {
            on_filter_validation_failed(session);
        });
        resource->set_error_handler([this](const int, const exception &e, const shared_ptr<Session> session){
            on_resource_error_handler(0,e,session);

        });
        _rest_service.publish(resource);


        resource = make_shared<Resource>();
        resource->set_paths({"start_amount_tasks", "start_tasks"});
        resource->set_method_handler("POST", filters, [this](shared_ptr<Session> session) {
            on_tasks_start(session);
        });
        resource->set_failed_filter_validation_handler([this](shared_ptr<Session> session) {
            on_filter_validation_failed(session);
        });
        resource->set_error_handler([this](const int, const exception &e, const shared_ptr<Session> session){
            on_resource_error_handler(0,e,session);

        });
        _rest_service.publish(resource);


        resource = make_shared<Resource>();
        resource->set_paths({"pause_task", "resume_task", "stop_task"});
        resource->set_method_handler("POST", filters, [this](shared_ptr<Session> session) {
            on_task_control(session);
        });
        resource->set_failed_filter_validation_handler([this](shared_ptr<Session> session) {
            on_filter_validation_failed(session);
        });
        resource->set_error_handler([this](const int, const exception &e, const shared_ptr<Session> session){
            on_resource_error_handler(0,e,session);

        });
        _rest_service.publish(resource);


        resource = make_shared<Resource>();
        resource->set_paths({"get_task_report"});
        resource->set_method_handler("GET", [this](shared_ptr<Session> session) {
            on_task_report(session);
        });
        resource->set_failed_filter_validation_handler([this](shared_ptr<Session> session) {
            on_filter_validation_failed(session);
        });
        resource->set_error_handler([this](const int, const exception &e, const shared_ptr<Session> session){
            on_resource_error_handler(0,e,session);

        });
        _rest_service.publish(resource);


        resource = make_shared<Resource>();
        resource->set_paths({"get_minute_report"});
        resource->set_method_handler("GET", [this](shared_ptr<Session> session) {
            on_minute_report(session);
        });
        resource->set_failed_filter_validation_handler([this](shared_ptr<Session> session) {
            on_filter_validation_failed(session);
        });
        resource->set_error_handler([this](const int, const exception &e, const shared_ptr<Session> session){
            on_resource_error_handler(0,e,session);

        });
        _rest_service.publish(resource);


        resource = make_shared<Resource>();
        resource->set_path("report");
        resource->set_method_handler("GET", [this](shared_ptr<Session> session) {
            on_websocket_connected(session);
        });
        resource->set_error_handler([this](const int, const exception &e, const shared_ptr<Session> session){
            on_resource_error_handler(0,e,session);

        });
        _rest_service.publish(resource);


        _rest_service.schedule([this]() {
            on_schedule();
        }, 10s);
        _permission_server_info = Configuration::instance().get_permission_server_info();
        auto info = Configuration::instance().get_rest_server_info();
        auto server_ip = info.first;
        auto server_port = info.second;
        auto settings = make_shared<Settings>();
        settings->set_bind_address(server_ip);
        settings->set_port(server_port);
        settings->set_connection_timeout(30s);
        settings->set_worker_limit(thread::hardware_concurrency());
        settings->set_default_header("Connection", "close");
        _rest_service.set_error_handler([this](const int, const exception &e, const shared_ptr<Session> session){
            on_service_error_handler(0, e, session);
        });
        _rest_service.start(settings);
    }

    void RESTServer::stop() {
        _rest_service.stop();
    }

    void RESTServer::on_rest_response(const json &data, shared_ptr<Session> session) {
        string response = data.dump();
        const multimap<string, string> headers{
                {"Content-Length", to_string(response.size())},
                {"Content-Type",   "application/json"}
        };
        if (data["code"] > 0)
            LOG(ERROR) << "request error: " << data["code"] << " | " << data["message"];
        else
            LOG(INFO) << "request info send successd";
        session->close(OK, response, headers);
    }

    void RESTServer::on_rest_response(const string &data, shared_ptr<Session> session) {
        const multimap<string, string> headers{
                {"Content-Length", to_string(data.size())},
                {"Content-Type",   "application/json"}
        };
        LOG(INFO) << "requests info send successd";
        session->close(OK, data, headers);
    }

    void RESTServer::print(const shared_ptr<Response> &response) {
        fprintf(stderr, "*** Response ***\n");
        fprintf(stderr, "Status Code:    %i\n", response->get_status_code());
        fprintf(stderr, "Status Message: %s\n", response->get_status_message().data());
        fprintf(stderr, "HTTP Version:   %.1f\n", response->get_version());
        fprintf(stderr, "HTTP Protocol:  %s\n", response->get_protocol().data());
        for (const auto &header : response->get_headers()) {
            fprintf(stderr, "Header '%s' > '%s'\n", header.first.data(), header.second.data());
        }
        const size_t length = (size_t)response->get_header("Content-Length", 10);
        Http::fetch(length, response);
        fprintf(stderr, "Body: %.*s...\n\n", 25, response->get_body().data());
    }

    // TODO: get user permission
    bool RESTServer::get_user_permission(const string &username) {
        auto url = "http://" + _permission_server_info.first + ":"
                   + to_string(_permission_server_info.second)
                   + "/proxycenter/v1/algoAuthority?userName=" + username;
        LOG(INFO) << url;
        try {
            auto request = make_shared<Request>(Uri(url));
            request->set_method("GET");
            request->set_header("Accept", "*/*");
            request->set_header("Content-Type", "*/*");
            request->set_header("Host",
                                _permission_server_info.first + ":" + to_string(_permission_server_info.second));
            auto response = Http::sync(request);
            auto length = (size_t) response->get_header("Content-Length", 10);
            Http::fetch(length, response);
            // TODO: Transfer-Encoding: chunked
            // Http::fetch("\r\n", response);
            auto rsp = response->get_body();
            auto str = string(rsp.begin(), rsp.end());
            auto idx = str.find("true");
            if (idx != string::npos) {
                LOG(INFO) << "has permission: " << str;
                return true;
            }
            LOG(INFO) << "has no permission: " << str;
            return false;
        } catch (exception &e) {
            LOG(ERROR) << "get permission error: " << e.what();
            return false;
        }
    }

    void RESTServer::on_user_login(shared_ptr<Session> session) {
        LOG(INFO) << "start to login muse" << session->get_origin();
        auto request = session->get_request();
        LOG(INFO) << "User from :" << session->get_origin() << " try to login" << endl;
        size_t content_length = (size_t) request->get_header("Content-Length", 0);
        session->fetch(content_length, [&](shared_ptr<Session> session, const Bytes &body) {
            string username;
            string password;
            string server_ip;
            string user_mac = {};
            int server_port = 0;
            try {
                auto param = json::parse(string(body.begin(), body.end()));
                username = param["username"].get<string>();
                password = param["password"].get<string>();
                server_ip = param["server_ip"].get<string>();
                server_port = param["server_port"].get<int>();

                //TODO version 1.7.1 use
                if(param.find("decryptedMac") != param.end()) {
                    user_mac = param["decryptedMac"].get<string>();
                }
                //TODO next version
//                if(param.find("decryptedMac") != param.end()) {
//                    user_mac = param["decryptedMac"].get<string>();
//                    if ("" == user_mac) {
//                     throw ErrorCode::CANNOT_GET_MAC;
//                        LOG(ERROR) << "Can not get macaddress" << "| username: " << username;
//                    }
//                } else {
//                    throw ErrorCode::CANNOT_GET_MAC;
//                    LOG(ERROR) << "Can not get macaddress" << "| username: " << username;
//                }
            } catch (const json::exception &e) {
                auto error = ErrorCategory(ErrorCode::WRONG_FORMAT);
                json data;
                data["code"] = error.value();
                data["message"] = error.message();
                on_rest_response(data, session);
                return;
            } catch (const ErrorCategory &e_msg) {
                json data;
                data["code"] = e_msg.value();
                data["message"] = e_msg.message();
                on_rest_response(data, session);
            }

            bool user_login_permission = get_user_permission(username);

#ifdef MY_DEBUG
            user_login_permission = true;
#endif

            if (user_login_permission) {
                LOG(INFO) << "start to login xtp";
                auto result = UserManager::instance().user_login(username, password, server_ip, server_port, user_mac);
                json data;
                auto code = result.first;
                data["code"] = code.value();
                data["message"] = code.message();
                if (code.get_error_code() == ErrorCode::NO_ERROR) {
                    data["session_id"] = result.second;
                }
                _user_ids[username].push_back(0);
                on_rest_response(data, session);
            } else {
                LOG(ERROR) << "permission denied for user: " << username;
                auto error = ErrorCategory (ErrorCode::PERMISSION_DENIED);
                json data;
                data["code"] = error.value();
                data["message"] = error.message();
                on_rest_response(data, session);
            }
        });
    }

    void RESTServer::on_user_logout(shared_ptr<Session> session) {
        LOG(INFO) << "start to logout muse" << session->get_origin();
        auto request = session->get_request();
        size_t content_length = (size_t) request->get_header("Content-Length", 0);
        session->fetch(content_length, [&](shared_ptr<Session> session, const Bytes &body) {
            string cli_session_id;
            try {
                auto param = json::parse(string(body.begin(), body.end()));
                cli_session_id = param["session_id"].get<string>();
            } catch (const json::exception &e) {
                json data;
                auto error = ErrorCategory(ErrorCode::WRONG_FORMAT);
                data["code"] = error.value();
                data["message"] = error.message();
                on_rest_response(data, session);
                return;
            }
            LOG(INFO) << "start to logout xtp";
            auto code = UserManager::instance().user_logout(cli_session_id);
            json data;
            data["code"] = code.value();
            data["message"] = code.message();
            on_rest_response(data, session);
        });
    }

    void RESTServer::on_task_start(shared_ptr<Session> session) {
        auto request = session->get_request();
        auto path = request->get_path();
        LOG(INFO) << path << " | " << session->get_origin();
        size_t content_length = (size_t) request->get_header("Content-Length", 0);
        session->fetch(content_length, [&,path](shared_ptr<Session> session, const Bytes &body) {
            json data;
            json output_data;
            string port_name;
            if (path == "/start_amount_task") {
                port_name = "start_amount_task";
            }
            if(path == "/start_task" ) {
                port_name = "start_task";
            }
            try {
                auto param = json::parse(string(body.begin(), body.end()));
                data = basic_task_parser(param, port_name);
                on_rest_response(data, session);
            } catch (const json::exception &e) {
                auto  error = ErrorCategory(ErrorCode::WRONG_FORMAT);
                data["code"] = error.value();
                data["message"] = error.message();
                on_rest_response(data, session);
            } catch (const ErrorCategory  &e_msg) {
                data["code"] = e_msg.value();
                data["message"] = e_msg.message();
                on_rest_response(data, session);
            } catch (const exception &e) {
                LOG(ERROR) << "request error: " << e.what();
                auto error = ErrorCategory(ErrorCode::UNKNOWN_ERROR);
                data["code"] = error.value();
                data["message"] = error.message();
                on_rest_response(data, session);
            }
        });
    }

    void RESTServer::on_tasks_start(shared_ptr<Session> session) {
        auto request = session->get_request();
        auto path = request->get_path();
        LOG(INFO) << path << " | " << session->get_origin();
        size_t content_length = (size_t) request->get_header("Content-Length", 0);
        session->fetch(content_length, [&, path](shared_ptr<Session> session, const Bytes &body) {
            list<thread> thread_list;
            set<int> user_ids;
            auto tasks_response = make_shared<list<string>>();
            json data;
            string port_name;
            if (path == "/start_amount_tasks") {
                port_name = "start_amount_tasks";
            }
            if(path == "/start_tasks" ) {
                port_name = "start_tasks";
            }
            try {
                auto parse_iter = json::parse(string(body.begin(), body.end()));
                LOG(INFO) << "tasks numbers: |" << parse_iter.size();
                if ((int)parse_iter.size()>_max_tasks_size){

                    throw ErrorCategory(ErrorCode::OVER_TASKS_LIMIT);
                }

                for (auto param : parse_iter) {
                    thread thread_parse(&RESTServer::_tasks_unit, this, param, tasks_response, port_name);
                    thread_list.push_back(move(thread_parse));
                }
                for (auto &th : thread_list) {
                    th.join();
                }
            } catch (const json::exception &e) {
                auto error = ErrorCategory (ErrorCode::WRONG_FORMAT);
                data["code"] = error.value();
                data["message"] = error.message();
                tasks_response->push_back(data.dump());
            } catch (const ErrorCategory  &e_msg) {
                data["code"] = e_msg.value();
                data["message"] = e_msg.message();
                tasks_response->push_back(data.dump());
            } catch (const exception &e) {
                LOG(ERROR) << "request error: " << e.what();
                auto error = ErrorCategory(ErrorCode::UNKNOWN_ERROR);
                data["code"] = error.value();
                data["message"] = error.message();
                tasks_response->push_back(data.dump());
            }
            on_rest_response(list_to_string(*tasks_response), session);
        });

    }

    void RESTServer::_tasks_unit(json param, shared_ptr<list<string>> tasks_response, string port_name) {

        json data = {};
        try {

            data = basic_task_parser(move(param), move(port_name));
        } catch (const json::exception &e) {
            auto error = ErrorCategory(ErrorCode::WRONG_FORMAT);
            data["code"] = error.value();
            data["message"] = error.message();
        } catch (const ErrorCategory  &e_msg) {
            data["code"] = e_msg.value();
            data["message"] = e_msg.message();
        }
        {
            unique_lock<shared_mutex> lock_guard(_tasks_report_mutex);
            tasks_response->push_back(data.dump());
        }
    }


    double RESTServer::get_market_data(string &ticker_id, MarketType &market) {
        auto ticker = make_pair(ticker_id, market);
        QuoteServer::instance().subscribe_market_data(ticker);
        auto market_data = QuoteServer::instance().query_market_data(ticker);
        double ticker_price = {};
        for(int i = 0; i<20; i++) {
            this_thread::sleep_for(0.01s);
            LOG(ERROR) << "ticker has no quote | " << ticker.first;
            market_data = QuoteServer::instance().query_market_data(ticker);
            if (market_data != nullptr){
                break;
            }
        }
        if (market_data == nullptr) {
            auto market_data_old = QuoteServer::instance().query_tickers_price_info(ticker);
            if (market_data_old== nullptr) {
                LOG(ERROR) << "can not get ticker price in ticker price info | " << ticker.first;
            } else {
                if(market_data_old->last_price <= 0.0){
                    LOG(ERROR) << "the price in ticker price info is zero | " << ticker.first;
                } else {
                    LOG(INFO) << "ticker price from ticker price info : " << market_data_old->last_price << " | stock: "
                              << ticker_id;
                    ticker_price = market_data_old->last_price;
                }
            }
        } else {
            LOG(INFO) << "ticker price from quoter: " << market_data->last_price << " | stock: " << ticker_id;
            ticker_price = market_data->last_price;
        }
        return ticker_price;
    }


    json RESTServer::basic_task_parser(json param, string port_name) {
        string cli_session_id;
        Type task_type = {};
        string ticker_id;
        MarketType market;
        int64_t quantity;
        SideType side;
        shared_ptr<AssetPositionCalculator> calculator;

        bool amount_flag = false;
        double ticker_price = 0.0;
        int max_qty = 1000000;
        uint64_t task_user_id = 0;

        json data;
        ErrorCategory error;
        BusinessType business_type = BusinessType::CASH;
        json task_params;
        string user_mac = {};


        try {

            LOG(INFO) << "parameter: " << param.dump();
            cli_session_id = param["session_id"].get<string>();
            task_type = Task::string_to_type(param["task_type"].get<string>());
            ticker_id = param["ticker"].get<string>();
            market = string_to_market_type(param["market"].get<string>());
            side = string_to_side_type(param["side"].get<string>());
            task_params["start_time"] = param["start_time"].get<string>();
            task_params["end_time"] = param["end_time"].get<string>();
            auto start_time = task_params["start_time"].get<string>();
            auto end_time = task_params["end_time"].get<string>();

            shared_ptr<User> user_tmp = UserManager::instance().find_user(cli_session_id);
            if (user_tmp == nullptr) {
                throw ErrorCategory(ErrorCode::WRONG_SESSION_ID);
            }

            if (param.find("business_type") != param.end()) {
                business_type = string_to_business_type(param["business_type"].get<string>());
                calculator = _create_calculator(business_type, market, side);
            } else {
                throw ErrorCategory(ErrorCode::WRONG_BUSINESS_TYPE);
            }

            if (param.find("maximal_qty") != param.end()) {
                max_qty = param["maximal_qty"].get<int64_t>();
            }
            task_params["maximal_qty"] = max_qty;

            if (param.find("id") != param.end()) {
                task_user_id = param["id"].get<uint64_t>();
                data["id"] = task_user_id;
            } else {
                data["id"] = 0;
            }

            if (!check_time_string(start_time) or !check_time_string(end_time)) {
                throw ErrorCategory(ErrorCode::WRONG_TIME_RANGE);
            }
            if (side == SideType::UNKNOWN or (side != SideType::BUY and side != SideType::SELL)) {
                throw ErrorCategory(ErrorCode::INVALID_SIDE_TYPE_PARAMETER);
            }
            if (market != MarketType::SH and market != MarketType::SZ) {
                throw ErrorCategory(ErrorCode::WRONG_MARKET_TYPE);
            }

            auto init_ticker_price = get_market_data(ticker_id, market);
            ticker_price = calculator->get_correct_price(init_ticker_price);
            if (init_ticker_price == 0.0) {
                throw ErrorCategory(ErrorCode::UNKNOWN_SECURITY);
            }

            int min_qty = calculator->get_minimal_qty();

            LOG(INFO) << port_name << endl;

            if (port_name == "start_amount_tasks" or port_name == "start_amount_task") {
                LOG(INFO) << "start_amount";
                check_parameters("amount", param, min_qty, max_qty, ticker_price, amount_flag, business_type);
                task_params["amount"] = param["amount"].get<double>();
                auto amount_value = param["amount"].get<double>();
                quantity = (int64_t) (calculator->amount_with_fee(amount_value) / ticker_price /
                                      calculator->get_minimal_qty()) * calculator->get_minimal_qty();
                task_params["quantity"] = quantity;
                check_parameters("quantity", task_params, min_qty, max_qty, ticker_price, amount_flag, business_type);
                amount_flag = true;
            } else {
                check_parameters("quantity", param, min_qty, max_qty, ticker_price, amount_flag, business_type);
                quantity = param["quantity"].get<int64_t>();
            }

            switch (task_type) {
                case Type::AIVWAP:
                    if (amount_flag) {
                        check_parameters("minimal_trade_amount", param, min_qty, max_qty, ticker_price, amount_flag,
                                         business_type);
                        task_params["minimal_trade_amount"] = param["minimal_trade_amount"].get<double>();
                    } else {
                        check_parameters("minimal_trade_quantity", param, min_qty, max_qty, ticker_price, amount_flag,
                                         business_type);
                        task_params["minimal_trade_quantity"] = param["minimal_trade_quantity"].get<int64_t>();
                    }
                    break;
                case Type::TWAP:
                    if (amount_flag) {
                        check_parameters("minimal_trade_amount", param, min_qty, max_qty, ticker_price, amount_flag,
                                         business_type);
                        task_params["minimal_trade_amount"] = param["minimal_trade_amount"].get<double>();
                    } else {
                        check_parameters("minimal_trade_quantity", param, min_qty, max_qty, ticker_price, amount_flag,
                                         business_type);
                        task_params["minimal_trade_quantity"] = param["minimal_trade_quantity"].get<int64_t>();
                    }
                    break;
                case Type::VWAP:
                    if (amount_flag) {
                        check_parameters("minimal_trade_amount", param, min_qty, max_qty, ticker_price, amount_flag,
                                         business_type);
                        task_params["minimal_trade_amount"] = param["minimal_trade_amount"].get<double>();
                    } else {
                        check_parameters("minimal_trade_quantity", param, min_qty, max_qty, ticker_price, amount_flag,
                                         business_type);
                        task_params["minimal_trade_quantity"] = param["minimal_trade_quantity"].get<int64_t>();
                    }
                    break;
                case Type::IS:
                    if (amount_flag) {
                        check_parameters("minimal_trade_amount", param, min_qty, max_qty, ticker_price, amount_flag,
                                         business_type);
                        task_params["minimal_trade_amount"] = param["minimal_trade_amount"].get<double>();
                    } else {
                        check_parameters("minimal_trade_quantity", param, min_qty, max_qty, ticker_price, amount_flag,
                                         business_type);
                        task_params["minimal_trade_quantity"] = param["minimal_trade_quantity"].get<int64_t>();
                    }
                    break;
                case Type::POV:
                    if (amount_flag) {
                        check_parameters("minimal_trade_amount", param, min_qty, max_qty, ticker_price, amount_flag,
                                         business_type);
                        task_params["minimal_trade_amount"] = param["minimal_trade_amount"].get<double>();
                    } else {
                        check_parameters("minimal_trade_quantity", param, min_qty, max_qty, ticker_price, amount_flag,
                                         business_type);
                        task_params["minimal_trade_quantity"] = param["minimal_trade_quantity"].get<int64_t>();
                    }
                    check_parameters("participation_rate", param, min_qty, max_qty, ticker_price, amount_flag,
                                     business_type);
                    //    v1.6.2 --  current participation_rate is double (0, 0.5) !!!!!!
                    task_params["participation_rate"] = param["participation_rate"].get<double>();
                    break;
                case Type::RANDOM:
                    if (amount_flag) {
                        check_parameters("minimal_trade_amount", param, min_qty, max_qty, ticker_price, amount_flag,
                                         business_type);
                        task_params["minimal_trade_amount"] = param["minimal_trade_amount"].get<double>();
                    } else {
                        check_parameters("minimal_trade_quantity", param, min_qty, max_qty, ticker_price, amount_flag,
                                         business_type);
                        task_params["minimal_trade_quantity"] = param["minimal_trade_quantity"].get<int64_t>();
                    }
                    check_parameters("num_of_tradings", param, min_qty, max_qty, ticker_price, amount_flag,
                                     business_type);
                    task_params["num_of_tradings"] = param["num_of_tradings"].get<int32_t>();
                    break;
                case Type::ICEBERG:
                    if (amount_flag) {
                        check_parameters("visible_amount", param, min_qty, max_qty, ticker_price, amount_flag,
                                         business_type);
                        task_params["visible_amount"] = param["visible_amount"].get<double>();
                        auto visible_amount = task_params["visible_amount"].get<double>();
                        task_params["visible_volume"] = (int64_t) ((floor)(visible_amount / ticker_price / min_qty) *
                                                                   min_qty);
                    } else {
                        check_parameters("visible_volume", param, min_qty, max_qty, ticker_price, amount_flag,
                                         business_type);
                        task_params["visible_volume"] = param["visible_volume"].get<int64_t>();
                    }
                    break;
                case Type::MSTD:
                    if (amount_flag) {
                        check_parameters("minimal_trade_amount", param, min_qty, max_qty, ticker_price, amount_flag,
                                         business_type);
                        task_params["minimal_trade_amount"] = param["minimal_trade_amount"].get<double>();
                    } else {
                        check_parameters("minimal_trade_quantity", param, min_qty, max_qty, ticker_price, amount_flag,
                                         business_type);
                        task_params["minimal_trade_quantity"] = param["minimal_trade_quantity"].get<int64_t>();
                    }
                    break;
                default:
                    throw ErrorCategory(ErrorCode::WRONG_TASK_TYPE);
            }
        } catch (const json::exception &e) {
            error = ErrorCategory(ErrorCode::WRONG_FORMAT);
            LOG(ERROR) << "task start error: " << error.value() << " | " << error.message();
        } catch (ErrorCategory &e) {
            error = e;
            LOG(ERROR) << "task start error: " << error.value() << " | " << error.message();
        }
        if (error.value() == 0) {
            if (auto user = UserManager::instance().find_user(cli_session_id)) {
                auto username = user->get_username();
                auto ticker = make_pair(ticker_id, market);
                LOG(INFO) << "ticker price: " << ticker_price << " | stock: " << ticker_id;
                auto qty = make_pair(quantity, side);
                auto task_id = RedisServer::instance().task_id_create();
                user_mac = user->find_mac(cli_session_id);
                if (empty(user_mac)) {
                    error = ErrorCategory(ErrorCode::ILLEGAL_SESSION);
                }
                auto task_input_data = make_shared<TaskInputData>(user, task_id, task_type, ticker, qty, business_type, amount_flag,
                                           task_user_id, user_mac);
                if (auto task = TaskFactory::create_task(task_input_data)) {
                    user->add_task(task);
                    task->set_calculator(calculator);
                    task->start(task_params);
                    data["task_id"] = task_id;
                    data["user_name"] = username;
                } else {
                    error = ErrorCategory(ErrorCode::FAILED_TO_CREATE_TASK);
                    LOG(ERROR) << "task start error: " << error.value() << " | " << error.message();
                }
            } else {
                error = ErrorCategory(ErrorCode::WRONG_SESSION_ID);
            }
        }
        data["code"] = error.value();
        data["message"] = error.message();
        return data;
    }

    bool RESTServer::check_parameters(const string &input_parameter, json &param, int min_qty, int max_qty,
                                      double price_base, bool &amount_flag, const BusinessType &business_type) {
        double price{};
        if (business_type == BusinessType::REPO) {
            price = 100;
        } else if (business_type == BusinessType::CASH) {
            price = price_base;
        } else {
            throw ErrorCategory(ErrorCode::WRONG_BUSINESS_TYPE);
        }


        if (input_parameter == "amount") {
            if (param.find("amount") != param.end()) {
                input_type_check(param["amount"], "double");
                auto amount_qty = param["amount"].get<double>() / price / min_qty * min_qty;

                if (amount_qty >= min_qty) {
                    return true;
                }
            }
            throw ErrorCategory(ErrorCode::INVALID_AMOUNT_PARAMETER);
        }

        if (input_parameter == "quantity") {
            if (param.find("quantity") != param.end()) {
                input_type_check(param["quantity"], "int");
                auto quantity = param["quantity"].get<int64_t>();
                if (quantity > 0 and quantity % min_qty == 0) {
                    return true;
                }
                throw ErrorCategory(ErrorCode::INVALID_QUANTITY_PARAMETER);
            }
        }

        if (input_parameter == "minimal_trade_amount") {
            if (param.find("minimal_trade_amount") != param.end()) {
                input_type_check(param["minimal_trade_amount"], "double");
                auto minimal_trade_amount_qty = param["minimal_trade_amount"].get<double>() / price / min_qty * min_qty;
                if (minimal_trade_amount_qty > 0 and
                    param["minimal_trade_amount"].get<double>() <= param["amount"].get<double>()
                    and minimal_trade_amount_qty <= max_qty) {
                    return true;
                }
                throw ErrorCategory(ErrorCode::INVALID_MINIMAL_TRADE_AMOUNT_PARAMETER);
            }
        }

        if (input_parameter == "minimal_trade_quantity") {
            if (param.find("minimal_trade_quantity") != param.end()) {
                input_type_check(param["minimal_trade_quantity"], "int");
                auto minimal_trade_quantity = param["minimal_trade_quantity"].get<int64_t>();
                if (minimal_trade_quantity > 0 and minimal_trade_quantity <= param["quantity"].get<int64_t>()
                    and minimal_trade_quantity % min_qty == 0 and minimal_trade_quantity <= max_qty) {
                    return true;
                }
                throw ErrorCategory(ErrorCode::INVALID_MINIMAL_TRADE_QUANTITY_PARAMETER);
            }
        }

        if (input_parameter == "participation_rate") {
            if (param.find("participation_rate") != param.end()) {
                input_type_check(param["participation_rate"], "double");
                auto participation_rate = param["participation_rate"].get<double>();
                if (participation_rate > 0.0 and participation_rate <= 0.5) {
                    return true;
                }
                throw ErrorCategory(ErrorCode::INVALID_PARTICIPATION_RATE_PARAMETER);
            }
        }

        if (input_parameter == "num_of_tradings") {
            if (param.find("num_of_tradings") != param.end()) {
                input_type_check(param["num_of_tradings"], "int");
                auto num_of_tradings = param["num_of_tradings"].get<int32_t>();
                int64_t times = 0;
                if (amount_flag) {
                    times = (int64_t) (param["amount"].get<double>() /
                                       max(min_qty * price, param["minimal_trade_amount"].get<double>()));
                } else {
                    times = param["quantity"].get<int64_t>() /
                            max((int64_t) min_qty, param["minimal_trade_quantity"].get<int64_t>());
                }
                if (num_of_tradings > 0 and num_of_tradings <= times) {
                    return true;
                }
            }
            throw ErrorCategory(ErrorCode::INVALID_NUM_OF_TRADINGS_PARAMETER);
        }

        if (input_parameter == "visible_amount") {
            if (param.find("visible_amount") != param.end()) {
                input_type_check(param["visible_amount"], "double");
                auto visible_amount = param["visible_amount"].get<double>();
                auto visible_amount_qty = visible_amount / price / min_qty * min_qty;
                if (visible_amount > 0.0 and visible_amount_qty >= min_qty and visible_amount_qty <= max_qty
                    and visible_amount <= param["amount"].get<double>()) {
                    return true;
                }
            }
            throw ErrorCategory(ErrorCode::INVALID_VISIBLE_PARAMETERS);
        }

        if (input_parameter == "visible_volume") {

            if (param.find("visible_volume") != param.end()) {
                input_type_check(param["visible_volume"], "int");
                auto visible_volume = param["visible_volume"].get<int64_t>();
                if (visible_volume >= min_qty
                    and visible_volume % min_qty == 0
                    and visible_volume <= max_qty
                    and visible_volume <= param["quantity"].get<int64_t>()) {
                    return true;
                }
            }
            throw ErrorCategory(ErrorCode::INVALID_VISIBLE_PARAMETERS);
        }
        return false;
    }

    void RESTServer::on_task_control(shared_ptr<Session> session) {
        LOG(INFO) << "on task control" << session->get_origin();
        auto request = session->get_request();
        auto path = request->get_path();
        size_t content_length = (size_t) request->get_header("Content-Length", 0);
        session->fetch(content_length, [&, path](shared_ptr<Session> session, const Bytes &body) {
            json param;
            try {
                param = json::parse(string(body.begin(), body.end()));
                LOG(INFO) << "parameter: " << param.dump();
            } catch (const json::exception &e) {
                json data;
                auto error = ErrorCategory(ErrorCode::WRONG_FORMAT);
                data["code"] = error.value();
                data["message"] = error.message();
                on_rest_response(data, session);
                return;
            }
            auto cli_session_id = param["session_id"].get<string>();
            auto code = ErrorCategory(ErrorCode::NO_ERROR);
            if (auto user = UserManager::instance().find_user(cli_session_id)) {
                auto task_id = param["task_id"].get<uint32_t>();
                auto result = 0;
                if (auto task = user->find_task(task_id)) {
                    if (path == "/pause_task") {
                        result = task->pause(cli_session_id);
                        if (result == 1) {
                            code = ErrorCategory(ErrorCode::TASK_ALREADY_PAUSED);
                        }
                        if (result == 2) {
                            code = ErrorCategory(ErrorCode::FAILED_TO_PAUSE_TASK);
                        }
                    } else if (path == "/resume_task") {
                        result = task->resume(cli_session_id);
                        if (result == 1) {
                            code = ErrorCategory(ErrorCode::TASK_ALREADY_RESUMED);
                        }
                        if (result == 2) {
                            code = ErrorCategory(ErrorCode::FAILED_TO_RESUME_TASK);
                        }
                    } else if (path == "/stop_task") {
                        result = task->stop(cli_session_id);
                        if (result == 1) {
                            code = ErrorCategory(ErrorCode::TASK_ALREADY_STOPPED);
                        }
                        if (result == 2) {
                            code = ErrorCategory(ErrorCode::FAILED_TO_STOP_TASK);
                        }
                    } else {
                        code = ErrorCategory(ErrorCode::UNKNOWN_ERROR);
                    }
                } else {
                    code = ErrorCategory(ErrorCode::WRONG_TASK_ID);
                }
            } else {
                code = ErrorCategory(ErrorCode::WRONG_SESSION_ID);
            }
            json data;
            data["code"] = code.value();
            data["message"] = code.message();
            on_rest_response(data, session);
        });
    }

    void RESTServer::on_task_report(std::shared_ptr<restbed::Session> session) {
        LOG(INFO) << "ask for task report" << session->get_origin();
        json data;
        auto code = ErrorCategory(ErrorCode::NO_ERROR);
        const auto request = session->get_request();
        auto cli_session_id = request->get_query_parameter("session_id", "");
        auto username = request->get_query_parameter("username", "");
        if (username.empty() or cli_session_id.empty()) {
            LOG(INFO) << "ask for task report" << session->get_origin();
            code = ErrorCategory(ErrorCode::WRONG_FORMAT);
            data["code"] = code.value();
            data["message"] = code.message();
            on_rest_response(data, session);
            return;
        }
        LOG(INFO) << "ask for task report" << session->get_origin();
        if (auto user = UserManager::instance().find_user(cli_session_id)) {
            LOG(INFO) << "ask for task report" << session->get_origin();
            auto result = user->find_channel("")->read_last_history_report("task", username);
            data["code"] = code.value();
            data["message"] = vector_to_string(result);
            string response = data.dump();
            session->close(OK, response, {{"Content-Length", to_string(response.size())},
                                          {"Connection",     "close"}});
        } else {
            LOG(INFO) << "ask for task report" << session->get_origin();
            code = ErrorCategory (ErrorCode::WRONG_SESSION_ID);
            data["code"] = code.value();
            data["message"] = code.message();
            string response = data.dump();
            LOG(WARNING) << "on task report request unauthorized | " << code.value() << " | " << code.message();
            session->close(UNAUTHORIZED, response, {{"Content-Length", to_string(response.size())},
                                                    {"Connection",     "close"}});
        }

    }

    void RESTServer::on_minute_report(shared_ptr<Session> session) {
        LOG(INFO) << "ask for minute report | IP: " << session->get_origin();
        json data;
        auto code = ErrorCategory(ErrorCode::NO_ERROR);
        const auto request = session->get_request();
        uint32_t task_id = (uint32_t) stoi(request->get_query_parameter("task_id", "0"));
        auto cli_session_id = request->get_query_parameter("session_id", "");
        auto username = request->get_query_parameter("username", "");
        LOG(INFO) << "ask for minute report | IP" << session->get_origin();
        if (username.empty() or cli_session_id.empty() or task_id == 0) {
            LOG(INFO) << "ask for minute report" << session->get_origin();
            code = ErrorCategory(ErrorCode::WRONG_FORMAT);
            data["code"] = code.value();
            data["message"] = code.message();
            on_rest_response(data, session);
            return;
        }
        if (auto user = UserManager::instance().find_user(cli_session_id)) {
            LOG(INFO) << "ask for minute report" << session->get_origin();
            auto stream_name = "minute_report." + to_string(task_id);
            auto result = user->find_channel("")->read_minute_report(stream_name, username, task_id, 0, 0, 0);
            data["message"] = vector_to_string(result);
            data["code"] = code.value();
            string response = data.dump();
            session->close(OK, response, {{"Content-Length", to_string(response.size())},
                                          {"Connection",     "close"}});
        } else {
            LOG(INFO) << "ask for minute report" << session->get_origin();
            code = ErrorCategory(ErrorCode::WRONG_SESSION_ID);
            data["code"] = code.value();
            data["message"] = code.message();
            string response = data.dump();
            LOG(WARNING) << "on task report request unauthorized | " << code.value() << " | " << code.message();
            session->close(UNAUTHORIZED, response, {{"Content-Length", to_string(response.size())},
                                                    {"Connection",     "close"}});
        }

    }


    void RESTServer::on_websocket_connected(shared_ptr<Session> session) {
        LOG(INFO) << "start to set up websocket connection" << session->get_origin();
        auto request = session->get_request();
        auto cli_session_id = request->get_query_parameter("session_id", "");
        if (auto user = UserManager::instance().find_user(cli_session_id)) {
            auto connection = request->get_header("connection", String::lowercase);
            if (connection.find("upgrade") not_eq string::npos) {
                auto upgrade = request->get_header("upgrade", String::lowercase);
                if (upgrade == "websocket") {
                    multimap<string, string> headers;
                    headers.insert(make_pair("Upgrade", "websocket"));
                    headers.insert(make_pair("Connection", "Upgrade"));
                    auto protocol = request->get_header("Sec-WebSocket-Protocol");
                    if (!protocol.empty()) {
                        headers.insert(make_pair("Sec-WebSocket-Protocol", protocol));
                    }
                    auto sec_key = request->get_header("Sec-WebSocket-Key");
                    sec_key.append("258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
                    char buffer[SHA_DIGEST_LENGTH];
                    SHA1((const unsigned char *) sec_key.data(), sec_key.length(), (unsigned char *) buffer);
                    auto hash = base64_encode(string(buffer, SHA_DIGEST_LENGTH));
                    headers.insert(make_pair("Sec-WebSocket-Accept", hash));
                    session->upgrade(SWITCHING_PROTOCOLS, headers, [=](shared_ptr<WebSocket> socket) {
                        if (socket->is_open()) {
                            auto channel = make_shared<WebSocketChannel>(user, socket);
                            user->add_channel(channel);
                            lock_guard<shared_mutex> guard(_shared_mutex);
                            _websockets.push_back(socket);
                        }
                    });
                    return;
                }
            }
            LOG(WARNING) << "websocket connect bad request";
            session->close(BAD_REQUEST);
        } else {
            json data;
            auto code = ErrorCategory (ErrorCode::WRONG_SESSION_ID);
            data["code"] = code.value();
            data["message"] = code.message();
            string response = data.dump();
            const multimap<string, string> headers{
                    {"Content-Length", to_string(response.size())},
                    {"Content-Type",   "application/json"}
            };
            LOG(WARNING) << "websocket connect unauthorized | " << code.value() << " | " << code.message();
            session->close(UNAUTHORIZED, response, headers);
        }
    }

    void RESTServer::on_filter_validation_failed(shared_ptr<Session> session) {
        LOG(WARNING) << "rest connect bad request";
        session->close(BAD_REQUEST);
    }

    void RESTServer::on_resource_error_handler(const int ,const exception& e, const shared_ptr<Session> session){
        if(session == nullptr) {
            LOG(ERROR) << "In resource_error_handle, the Session is null ";
        }
        if(session->is_open()){
            session->close(6000, "Custom Resource Internal Server Error", {{"Content-Length","37"}});
        } else{
            LOG(ERROR)<< "Custom Resource Internal Server Error";
        }

    }

    void RESTServer::on_service_error_handler(const int ,const  exception& e, const shared_ptr<Session> session){

        if (session == nullptr) {
            LOG(ERROR) << "In server_error_handler, the Session is null!";
        }
        if(session->is_open()){
            session->close(5000, "Custom Service Internal Server Error", {{"Content-Length","36"}});
        } else{
            LOG(ERROR)<< "Custom Service Internal Server Error";
        }

    }

    void RESTServer::on_schedule() {
        lock_guard<shared_mutex> guard(_shared_mutex);
        try {
            auto i = _websockets.begin();
            while (i not_eq _websockets.end()) {
                auto &socket = *i;
                if (socket->is_open()) {
                    LOG(INFO) << "send websocket ping";
                    socket->send(WebSocketMessage::PING_FRAME);
                    i++;
                } else {
                    LOG(WARNING) << "this Websocket not exist";
                    socket->close();
                    i = _websockets.erase(i);
                }
            }
        } catch (const std::exception &ex) {
            LOG(ERROR) << "Websocket on schedule failed" << ex.what();
        }
    }

    string RESTServer::vector_to_string(const vector<string> &report) {
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

    string RESTServer::list_to_string(const list<string> &report) {
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


    shared_ptr<AssetPositionCalculator>
    RESTServer::_create_calculator(BusinessType &businessType, MarketType &marketType,
                                   SideType &sideType) {

        if (businessType == BusinessType::REPO) {
            return make_shared<RepoCapitalCalculator>(sideType, marketType);
        } else if (businessType == BusinessType::CASH) {
            return make_shared<CashCapitalCalculator>(sideType);
        } else {
            throw ErrorCategory(ErrorCode::WRONG_BUSINESS_TYPE);
        }
    }

    bool RESTServer::check_time_string(string &time_str) {
        if (time_str.length() != 8) {
            return false;
        }
        if (time_str[2] not_eq ':' or time_str[5] not_eq ':') {
            return false;
        }
        return true;
    }

}

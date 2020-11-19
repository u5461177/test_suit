//
// Created by Rong Zhou on 2018-08-03.
//

#include "user.h"
#include "redisserver.h"
#include "traderserver.h"
#include "util/cryptohelper.h"
#include "types/errorcode.h"
#include "glog/logging.h"
#include "types/businesstype.h"
#include "configuration.h"

namespace ats {

    using namespace std;
    using namespace std::chrono;

    User::User(const string &username) : _username(username) {
        _xtp_session_id = 0;
        RedisServer::instance().on_user_created(username);
    }

    User::~User() {
        LOG(INFO) << "start to exit user";
        RedisServer::instance().on_user_destroyed(_username);
        LOG(INFO) << "success exit user";
    }


    pair<ErrorCategory, string>
    User::login(const string &password, const string &server_ip, int server_port, const string &user_mac) {
        if (_xtp_session_id == 0) {
            auto result = TraderServer::instance().connect(_username, password, server_ip, server_port);
            if (result.first.get_error_code() not_eq ErrorCode::NO_ERROR) {
                return make_pair(result.first, "");
            }
            lock_guard<mutex> guard(_mutex);
            _password = password;
            _server_ip = server_ip;
            _server_port = server_port;
            _xtp_session_id = result.second;
            RedisServer::instance().on_user_connected(_username, password, server_ip, _xtp_session_id);
        }
        if (server_ip not_eq _server_ip) {
            return make_pair(ErrorCategory(ErrorCode::WRONG_SERVER_IP), "");
        }
        if (password not_eq _password) {
            return make_pair(ErrorCategory(ErrorCode::WRONG_PASSWORD), "");
        }
        auto text = generate_random_text();
        auto cli_session_id = hmac_encode(text);
        lock_guard<mutex> guard(_mutex);
        _cli_session_ids.push_back(cli_session_id);
        if (!empty(user_mac)) {
            _cli_sesssion_id_mac[cli_session_id] = user_mac;
        } else {
            _cli_sesssion_id_mac[cli_session_id] = "No Mac";
        }
        RedisServer::instance().on_user_login(_username, cli_session_id);
        return make_pair(ErrorCategory(ErrorCode::NO_ERROR), cli_session_id);
    }

    pair<ErrorCategory, string> User::relogin(const string &password, const string &server_ip, int server_port) {
        LOG(INFO) << "In relogin";
        if(_xtp_session_id == 0) {

            auto result = TraderServer::instance().connect(_username,password,server_ip,server_port);

            if(result.first.get_error_code() == ErrorCode::NO_ERROR) {
                {
                    lock_guard<mutex> guard(_mutex);
                    _xtp_session_id = result.second;
                }
                RedisServer::instance().on_user_connected(_username, password, server_ip, _xtp_session_id);
                return make_pair(result.first, "");
            }
            return make_pair(ErrorCategory(ErrorCode::FAILED_TO_RECONNECT),"");
        }
        return make_pair(ErrorCategory(ErrorCode::UNKNOWN_ERROR),"");

    }

    void User::relogin() {
        {  LOG(INFO) << "try to relogin" <<endl;
            lock_guard<mutex> guard(_mutex);
            _cli_session_ids = {};
            _password.clear();
            _server_ip.clear();
            _xtp_session_id = 0;
        }
        for_each(_channels.begin(), _channels.end(), [&](const shared_ptr<Channel> channel) {
            channel->send_user_relogin(_username);
        });
        _channels = {};
    }

    ErrorCategory User::logout(const string &cli_session_id) {
        lock_guard<mutex> guard(_mutex);
        auto i = find(_cli_session_ids.begin(), _cli_session_ids.end(), cli_session_id);
        if (i not_eq _cli_session_ids.end()) {
            _cli_sesssion_id_mac.erase(cli_session_id);
            _cli_session_ids.erase(i);
            RedisServer::instance().on_user_logout(_username, cli_session_id);
            if (_cli_session_ids.empty() and _tasks.empty()) {
                TraderServer::instance().disconnect(_xtp_session_id);
                _password.clear();
                _server_ip.clear();
                _xtp_session_id = 0;
                RedisServer::instance().on_user_disconnected(_username);
            }
            return ErrorCategory(ErrorCode::NO_ERROR);
        }
        return ErrorCategory(ErrorCode::WRONG_SESSION_ID);
    }

    shared_ptr<UserAsset> User::query_asset() {
        lock_guard<mutex> guard(_mutex);
        if (_xtp_session_id == 0) {
            LOG(INFO) << "can not connect XTP Trader Server";
            return nullptr;
        }
        LOG(INFO) << "query asset";
        if (_user_asset.first and system_clock::now() - _user_asset.second < 60s) {
            return _user_asset.first;
        }
        TraderServer::instance().query_user_asset(_xtp_session_id);
        return nullptr;
    }

    shared_ptr<StockPosition> User::query_stock_position(const string &ticker_id) {
        lock_guard<mutex> guard(_mutex);
        if (_xtp_session_id == 0) {
            LOG(INFO) << "can not connect XTP Trader Server";
            return nullptr;
        }
        auto i = find_if(_stock_positions.begin(), _stock_positions.end(), [=](const auto &stk_position) {
            return stk_position.first->ticker == ticker_id;
        });

        if (i not_eq _stock_positions.end() and system_clock::now() - i->second < 60s) {
            return i->first;
        }
        TraderServer::instance().query_stock_position(_xtp_session_id, ticker_id);

        return nullptr;
    }

    void User::add_task(shared_ptr<TaskProxy> task) {
        lock_guard<mutex> guard(_mutex);
        _tasks.push_back(task);
        RedisServer::instance().on_user_add_task(_username, task->get_task_id());
    }

    void User::remove_task(uint32_t task_id) {
        lock_guard<mutex> guard(_mutex);
        RedisServer::instance().on_user_remove_task(_username, task_id);
        RedisServer::instance().on_task_destroyed(task_id);
        RedisServer::instance().on_report_destroyed(task_id);

    }

    shared_ptr<TaskProxy> User::find_task(uint32_t task_id) {
        lock_guard<mutex> guard(_mutex);
        auto i = find_if(_tasks.begin(), _tasks.end(), [task_id](shared_ptr<TaskProxy> task) {
            return task->get_task_id() == task_id;
        });
        return i not_eq _tasks.end() ? *i : nullptr;
    }

    shared_ptr<TaskProxy> User::find_task(uint64_t xtp_order_id) {
        lock_guard<mutex> guard(_mutex);
        auto i = _xtp_order_id_tasks.find(xtp_order_id);
        return i not_eq _xtp_order_id_tasks.end() ? i->second : nullptr;
    }

    size_t User::count_task() {
        return _tasks.size();
    }

    void User::add_channel(shared_ptr<Channel> channel) {
        lock_guard<mutex> guard(_mutex);
        _channels.push_back(channel);
        LOG(INFO) << "channel id: " << channel->get_channel_id();
        RedisServer::instance().on_user_add_channel(_username, channel->get_channel_id());
    }

    void User::remove_channel(const string &channel_id) {
        lock_guard<mutex> guard(_mutex);
        _channels.erase(remove_if(_channels.begin(), _channels.end(), [channel_id](shared_ptr<Channel> channel) {
            return channel->get_channel_id() == channel_id;
        }), _channels.end());
        RedisServer::instance().on_user_remove_channel(_username, channel_id);
    }

    shared_ptr<Channel> User::find_channel(const string &channel_id) {
        lock_guard<mutex> guard(_mutex);
        auto i = find_if(_channels.begin(), _channels.end(), [=](shared_ptr<Channel> channel) {
            return channel->get_channel_id() == channel_id;
        });
        return i not_eq _channels.end() ? *i : nullptr;
    }

    string User::find_mac(const string &cli_session_id) {

        auto cli_itr = _cli_sesssion_id_mac.find(cli_session_id);
        if (cli_itr != _cli_sesssion_id_mac.end()) {
            return cli_itr->second;
        }

        return "";
    }

    uint64_t
    User::place_order(uint32_t task_id, const Ticker &ticker, const Price &price, const Quantity &quantity,
                      const BusinessType &businessType) {

        if (_xtp_session_id == 0) {
            LOG(INFO) << "can not connect XTP Trader Server";
            return 0;
        }
        unsigned long xtp_order_id = 0;
        xtp_order_id = TraderServer::instance().place_order(task_id, _xtp_session_id, ticker, price, quantity,
                                                                 businessType);
        auto task = find_task(task_id);
        assert(task != nullptr);
        lock_guard<mutex> guard(_mutex);
        _xtp_order_id_tasks[xtp_order_id] = task;
        gettimeofday(&_t_place, nullptr);
        long long time_point = 1000000 * _t_place.tv_sec + _t_place.tv_usec;
        _t_places[xtp_order_id] = time_point;
        auto marketquotation = make_shared<MarketQuotationData>(task->get_current_market_data());

        task->set_xtp_order_id_market_data(quantity.second,price.first, xtp_order_id, marketquotation);

        return xtp_order_id;
    }

    void User::cancel_order(uint64_t xtp_order_id) {
        if (_xtp_session_id == 0) {
            LOG(INFO) << "can not connect XTP Trader Server";
        } else {
            TraderServer::instance().cancel_order(_xtp_session_id, xtp_order_id);
        }
    }

    void User::on_asset(shared_ptr<UserAsset> asset, bool get_from_xtp) {
        lock_guard<mutex> guard(_mutex);
        LOG(WARNING) << "no asset";
        if (get_from_xtp) {
            _user_asset = make_pair(asset, system_clock::now());
        } else {
            _user_asset = make_pair(asset, _user_asset.second);
        }
    }

    bool User::update_buying_power(double cost) {
        if (_user_asset.first != nullptr) {
            lock_guard<mutex> guard(_mutex);
            _user_asset.first->buying_power += cost;
            return true;
        }
        return false;
    }

    bool User::update_sellable_position(int64_t quantity_cost, shared_ptr<ats::StockPosition> stock_unit) {

        if (stock_unit != nullptr) {
            lock_guard<mutex> guard(_mutex);
            stock_unit->sellable_qty += quantity_cost;
            return true;
        }
        return false;

    }


    void User::on_stock_position(const string &ticker_id, shared_ptr<StockPosition> stk_position, bool get_from_xtp) {
        lock_guard<mutex> guard(_mutex);
        auto i = find_if(_stock_positions.begin(), _stock_positions.end(), [ticker_id](const auto &stk_position) {
            return stk_position.first->ticker == ticker_id;
        });
        if (i not_eq _stock_positions.end()) {
            if (get_from_xtp) {
                *i = make_pair(stk_position, system_clock::now());
            } else {
                *i = make_pair(stk_position, i->second);
            }
        } else {
            _stock_positions.emplace_back(make_pair(stk_position, system_clock::now()));
        }
    }

    void User::on_cancel_order_info(shared_ptr<OrderCancelInfo> cancel_order_info, shared_ptr<ErrorInfo> error_info) {
        LOG(INFO)<< "on cancel order info | " << cancel_order_info->order_xtp_id<< "error id: "<< error_info->error_id
        << " | error : "<< error_info->error_msg;
       if(auto task = find_task(cancel_order_info->order_xtp_id)){
           auto errorCode = ErrorCategory(ErrorCode::CANCEL_ORDER_ERROR);
            task->stop_and_set_msg(errorCode.message());
            //TODO send info to client?
//            for (const auto &channel : _channels) {
//                channel->send_order_cancel_info(_username, task->get_type(), task->get_task_id(), cancel_order_info,
//                        error_info);
//            }
        } else {
            LOG(INFO) << "not our order";
        }

    }

    void User::on_order_info(shared_ptr<OrderInfo> order_info_base, shared_ptr<ErrorInfo> error_info_base) {
        LOG(INFO) << "start to find task to send order info | order xtp id: " << order_info_base->order_xtp_id
                  << " | client_id:" << order_info_base->order_client_id ;

        if( auto founded_task = find_task(order_info_base->order_client_id)) {
            _xtp_order_id_list.emplace_back(make_pair(make_pair(order_info_base, error_info_base), 30));
        }

        for (auto order_it = _xtp_order_id_list.begin(); order_it != _xtp_order_id_list.end();) {
            auto order_info = order_it->first.first;
            auto error_info = order_it->first.second;
            if (auto task = find_task(order_info->order_xtp_id)) {
                LOG(INFO) << "on order info task id: " << task->get_task_id();
                task->on_order_info(order_info, error_info);
                auto order_uid = task->get_order_uid();
                lock_guard<mutex> guard(_mutex);
                gettimeofday(&_t_order, nullptr);
                for (const auto &channel : _channels) {
                    channel->send_order_info(_username, task->get_type(), task->get_task_id(), order_info,
                            error_info,
                            order_uid, task->find_market_quotation(order_info->order_xtp_id));
                }
                auto it = _t_orders.find(order_info->order_xtp_id);
                if (it == _t_orders.end()) {
                    long long time_point = 1000000 * _t_order.tv_sec + _t_order.tv_usec;
                    _t_orders[order_info->order_xtp_id] = time_point;
                }
                _xtp_order_id_list.erase(order_it++);
                LOG(INFO) << "order info send to channel successfully";
            } else {
                if (order_it->second == 0) {
                    _xtp_order_id_list.erase(order_it++);
                } else {
                    order_it->second--;
                    order_it++;
                }
            }
        }

    }

    void User::on_trade_report(shared_ptr<TradeReport> trade_report_base) {
        LOG(INFO) << "on trade info task id | order xtp id: "
                  << trade_report_base->order_xtp_id << " |client_id: " << trade_report_base;
        if (auto founded_task = find_task(trade_report_base->order_client_id)) {
            _xtp_trade_id_list.emplace_back(make_pair(trade_report_base,30));
        }
        for(auto trade_it = _xtp_trade_id_list.begin(); trade_it != _xtp_trade_id_list.end();) {
            auto trade_report = trade_it->first;
            if (auto task = find_task(trade_report->order_xtp_id)) {
                LOG(INFO) << "on trade info task id: " << task->get_task_id();
                task->on_trade_report(trade_report);
                auto trade_uid = task->get_trade_uid();
                lock_guard<mutex> guard(_mutex);
                gettimeofday(&_t_trade, nullptr);
                for (const auto &channel : _channels) {
                    channel->send_trade_report(_username, task->get_type(), task->get_task_id(), trade_report,
                                               trade_uid);
                }
                auto it = _t_trades.find(trade_report->order_xtp_id);
                if (it == _t_trades.end()) {
                    long long time_point = 1000000 * _t_trade.tv_sec + _t_trade.tv_usec;
                    _t_trades[trade_report->order_xtp_id] = time_point;
                }
                _xtp_trade_id_list.erase(trade_it++);
                LOG(INFO) << "trade info send to channel successfully";
            } else {
                if (trade_it->second == 0){
                    _xtp_trade_id_list.erase(trade_it++);
                } else{
                    trade_it->second--;
                    trade_it++;
                }
            }
        }

    }


    void User::on_task_destroyed(uint32_t task_id) {
        remove_task(task_id);
    }

    void User::on_task_id(uint32_t task_id, uint64_t task_user_id) {
        lock_guard<mutex> guard(_mutex);
        for (const auto &channel : _channels) {
            channel->send_task_id(_username, task_id, task_user_id);
        }
    }


    void User::on_minute_report(uint32_t task_id, Type task_type, std::shared_ptr<MinuteReport> minute_report) {
        lock_guard<mutex> guard(_mutex);
        for (const auto &channel : _channels) {
            channel->send_minute_report(_username, task_type, task_id, minute_report);
        }
    }

    void User::on_task_report(uint32_t task_id, Type task_type, std::shared_ptr<TaskReport> task_report,
            const string user_mac) {
        lock_guard<mutex> guard(_mutex);
        for (const auto &channel : _channels) {
            channel->send_task_status(_username, task_type, task_id, task_report, user_mac);
        }
    }

    void User::write_time_difference(uint32_t task_id, const map<uint64_t, long long> &t_quotes) {
        map<uint64_t, vector<string>> time_results;
        long long quote_time = 0;
        long long place_time = 0;
        long long order_time = 0;
        long long trade_time = 0;
        for (auto &key: _t_places) {
            try {
                quote_time = t_quotes.at(key.first);
                place_time = _t_places.at(key.first);
                order_time = _t_orders.at(key.first);
                trade_time = _t_trades.at(key.first);
                vector<string> result{"key", to_string(key.first), "quote_time", to_string(quote_time), "place_time",
                                      to_string(place_time), "order_time", to_string(order_time), "trade_time",
                                      to_string(trade_time)};
                time_results[key.first] = result;
            } catch (exception &e) {
                LOG(INFO) << "has no key: " << key.first;
            }
        }
        RedisServer::instance().on_time_write(_username, time_results, task_id);
    }
}
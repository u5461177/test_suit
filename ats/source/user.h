//
// Created by rongzhou on 18-7-28.
//

#ifndef ATS_USER_H
#define ATS_USER_H

#include <atomic>
#include <vector>
#include <list>
#include <map>
#include <mutex>
#include <memory>
#include <system_error>
#include "userasset.h"
#include "stockposition.h"
#include "orderinfo.h"
#include "tradereport.h"
#include "taskreport.h"
#include "types/errorinfo.h"
#include "channel.h"
#include "redischannel.h"
#include "task.h"
#include "usermanager.h"
#include "taskproxy.h"
#include "ordercancelinfo.h"

namespace ats {

    class User {
    public:
        User(const std::string &username);

        virtual ~User();

    public:
        const std::string &get_username() const;

        std::pair<ErrorCategory, std::string>
        login(const std::string &password, const std::string &server_ip, int server_port, const std::string &user_mac);

        std::pair<ErrorCategory, std::string> reconnect();

        ErrorCategory logout(const std::string &cli_session_id);

        void relogin();

        std::pair<ErrorCategory, std::string>
        relogin(const std::string &password, const std::string &server_ip, int server_port);

        uint64_t get_xtp_session_id() const;

        std::shared_ptr<UserAsset> query_asset();

        std::shared_ptr<StockPosition> query_stock_position(const std::string &ticker_id);

        void add_task(std::shared_ptr<ats::TaskProxy> task);

        void remove_task(uint32_t task_id);

        std::shared_ptr<ats::TaskProxy> find_task(uint32_t task_id);

        std::shared_ptr<ats::TaskProxy> find_task(uint64_t xtp_order_id);

        std::string find_mac(const std::string &cli_session_id);

        size_t count_task();

        void add_channel(std::shared_ptr<Channel> channel);

        void remove_channel(const std::string &channel_id);

        std::shared_ptr<Channel> find_channel(const std::string &channel_id);

        uint64_t
        place_order(uint32_t task_id, const Ticker &ticker, const Price &price, const Quantity &quantity,
                    const BusinessType &businessType);

        void cancel_order(uint64_t xtp_order_id);


        bool update_buying_power(double cost);

        bool update_sellable_position(int64_t quantity_cost, std::shared_ptr<StockPosition> stock_unit);


        void write_time_difference(uint32_t task_id, const std::map<uint64_t, long long> &t_quotes);

        void on_disconnected();

        void on_asset(std::shared_ptr<UserAsset> asset, bool get_from_xtp);

        void
        on_stock_position(const std::string &ticker_id, std::shared_ptr<StockPosition> stk_position, bool get_from_xtp);

        void on_order_info(std::shared_ptr<OrderInfo> order_info, std::shared_ptr<ErrorInfo> error_info);

        void on_cancel_order_info(std::shared_ptr<OrderCancelInfo> cancel_order_info, std::shared_ptr<ErrorInfo> error_info);

        void on_trade_report(std::shared_ptr<TradeReport> trade_report);


        void on_minute_report(uint32_t task_id, Type task_type, std::shared_ptr<MinuteReport> minute_report);

        void on_task_report(uint32_t task_id, Type task_type, std::shared_ptr<TaskReport> task_report,
                const std::string user_mac) ;

        void on_task_destroyed(uint32_t task_id);

        void on_task_id(uint32_t task_id, uint64_t task_user_id);

        std::pair<std::shared_ptr<UserAsset>, std::chrono::system_clock::time_point> get_user_asset() const;

        std::vector<std::pair<std::shared_ptr<StockPosition>, std::chrono::system_clock::time_point>>
        get_stock_positions() const;

        std::vector<std::string> get_cli_session_ids();

        std::vector<std::shared_ptr<ats::TaskProxy>> get_tasks();


    private:
        std::string _username;
        std::string _password;
        std::string _server_ip;
        int _server_port;
        std::atomic<uint64_t> _xtp_session_id;
        std::vector<std::string> _cli_session_ids;
        std::map<std::string,std::string> _cli_sesssion_id_mac;
        std::pair<std::shared_ptr<UserAsset>, std::chrono::system_clock::time_point> _user_asset;
        std::vector<std::pair<std::shared_ptr<StockPosition>, std::chrono::system_clock::time_point>> _stock_positions;
        std::vector<std::shared_ptr<ats::TaskProxy>> _tasks;
        std::map<uint64_t, std::shared_ptr<ats::TaskProxy>> _xtp_order_id_tasks;
        std::vector<std::shared_ptr<Channel>> _channels;
        struct timeval _t_place, _t_order, _t_trade;
        std::map<uint64_t, long long> _t_places, _t_orders, _t_trades;
        std::mutex _mutex;
        std::list<std::pair<std::pair<std::shared_ptr<OrderInfo>,std::shared_ptr<ErrorInfo>>, int>> _xtp_order_id_list;
        std::list<std::pair<std::shared_ptr<TradeReport>,int>> _xtp_trade_id_list;
    };

    inline const std::string &User::get_username() const {
        return _username;
    }

    inline std::vector<std::pair<std::shared_ptr<StockPosition>, std::chrono::system_clock::time_point>>
    User::get_stock_positions() const {
        return _stock_positions;
    }

    inline std::pair<ErrorCategory, std::string> User::reconnect() {
        return relogin(_password, _server_ip, _server_port);
    }

    inline uint64_t User::get_xtp_session_id() const {
        return _xtp_session_id;
    }

    inline std::pair<std::shared_ptr<UserAsset>, std::chrono::system_clock::time_point> User::get_user_asset() const {
        return _user_asset;
    }

    inline void User::on_disconnected() {
        _xtp_session_id = 0;
    }

    inline std::vector<std::string> User::get_cli_session_ids() {
        return _cli_session_ids;
    }

    inline std::vector<std::shared_ptr<ats::TaskProxy>> User::get_tasks() {
        return _tasks;
    }

}

#endif //ATS_USER_H
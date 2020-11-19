//
// Created by rongzhou on 18-7-29.
//

#ifndef ATS_TASK_H
#define ATS_TASK_H

#include <string>
#include <atomic>
#include <mutex>
#include <thread>
#include <algorithm>
#include <json.hpp>
#include "marketdata.h"
#include "order.h"
#include "util/timehelper.h"
#include "types/errorcode.h"
#include "types/pricetype.h"
#include "marketquotationdata.h"
#include "assetpositioncalculator.h"
#include "taskinterface.h"
#include "taskinputdata.h"

namespace ats {

    using Ticker = std::pair<std::string, MarketType>;
    using Quantity = std::pair<int64_t, SideType>;

    class User;

    class Task : public TaskInterface {
    public:

        static Type string_to_type(const std::string &str);

        static std::string type_to_string(Type task_type);


        static std::string status_to_string(Status task_status);

    public:
        explicit Task(std::shared_ptr<TaskInputData> task_input);

        ~Task() override;

    public:

        uint32_t get_task_id() const override;

        size_t get_order_uid() override;

        size_t get_trade_uid() override;

        void set_calculator(std::shared_ptr<AssetPositionCalculator> calculator) override;

        void start(const nlohmann::json &params) override;

        int32_t pause(const std::string &cli_session_id) override;

        int32_t resume(const std::string &cli_session_id) override;

        int32_t stop(const std::string &cli_session_id) override;

        int32_t stop() override;

        void on_order_info(std::shared_ptr<OrderInfo> order_info, std::shared_ptr<ErrorInfo> error_info) override;

        void on_trade_report(std::shared_ptr<TradeReport> trade_report) override;

        std::pair<ats::PriceAction, std::shared_ptr<ats::MarketQuotationData>>
        find_market_quotation(uint64_t xtp_order_id) override;

        virtual void update_market_data(std::unique_ptr<MarketData> market_data);

        bool update_asset_and_stock_position();

        void update_order_status();

        void _update_placed_qty();

        bool place_order(double price, int64_t quantity);

        void _update_asset_position_left_amount(const std::shared_ptr<OrderInfo> &orderInfo);

        bool _update_asset(double trade_amount_with_fee, const SideType &side,
                           const BusinessType &business_type);

        bool _update_position(int64_t quantity, const std::string &ticker_id);

        bool cancel_order(uint64_t xtp_order_id);

        std::pair<uint64_t, int64_t> find_worst_order();

        void parameter_parse();

        void task_init();

        void market_data_init();

        void on_task_stop();

        void on_task_stop(const std::string &cli_session_id);

        bool on_task_update();

        void on_task_report(std::chrono::system_clock::time_point start_time,
                            std::chrono::system_clock::time_point end_time);

        void on_task_report(std::chrono::system_clock::time_point start_time,
                            std::chrono::system_clock::time_point end_time, const std::string &cli_session_id);

        void on_minute_report();

        bool on_minute_update(std::chrono::system_clock::duration &minute_update_time);

        void task_status_change(uint32_t task_id, Status task_status, const std::string &cli_session_id);

        void task_status_change(uint32_t task_id, Status task_status);

        void check_detail();

        int check_time_out(std::chrono::system_clock::time_point end_time) override;

        virtual bool _break_time_check();

        void cancel_cold_orders();

        void cancel_all_orders();

        std::chrono::system_clock::duration calc_time_span(std::chrono::system_clock::time_point tp) const;

        bool set_xtp_order_id_market_data(ats::SideType side, double price, unsigned long xtp_order_id,
                                          std::shared_ptr<ats::MarketQuotationData> marketquotationdata) override;

        std::shared_ptr<ats::MarketData> get_current_market_data() const override;

        void stop_and_set_msg(std::string remark_msg) override;

        std::string _find_mac(const std::string &cli_session_id);

    private:
        void _task_status_change_no_report(Status task_status, const std::string &cli_session_id);


    public:
        bool cancel_last_order_flag = true;
        std::weak_ptr<User> _owner;
        uint32_t _task_id;
        Ticker _ticker;
        BusinessType _business_type;
        Quantity _quantity;
        double _amount;
        std::string _user_mac;
        uint64_t _task_user_id;
        std::string _current_date;
        std::chrono::system_clock::time_point _start_time;
        std::chrono::system_clock::time_point _end_time;
        std::chrono::system_clock::time_point _input_start_time;
        std::chrono::system_clock::time_point _input_end_time;
        std::chrono::system_clock::time_point _open_time;
        std::chrono::system_clock::time_point _break_start_time;
        std::chrono::system_clock::time_point _break_end_time;
        std::chrono::system_clock::time_point _close_time;
        std::chrono::system_clock::duration _total_time;
        std::chrono::system_clock::duration _refer_report_freq;
        std::chrono::system_clock::duration _refer_cancel_freq;
        Status _status;
        std::map<uint64_t, std::unique_ptr<Order>> _xtp_order_id_orders;
        std::vector<std::tuple<uint64_t, int64_t, double>> _left_order_xtp_ids;
        double _this_party_best_price;
        double _other_party_best_price;
        double _ticker_trade_price;
        int64_t _minimal_trade_quantity;
        double _minimal_trade_amount;
        int64_t _traded_order_quantity;
        uint64_t _hot_order_quantity;
        uint64_t _cold_order_quantity;
        std::mutex _mutex;
        double _last_price;
        int64_t _placed_qty;
        double _traded_amount;
        double _traded_amount_with_fee;
        double _repo_traded_ticker_value;
        int64_t _withdraw_qty;
        int64_t _last_withdraw_qty;
        double _init_market_turnover;
        double _market_turnover;
        int64_t _init_market_qty;
        int64_t _market_qty;
        int64_t _pending_qty;
        double _pending_amount;
        double _pending_amount_with_fee;
        double _left_amount;
        std::atomic<size_t> _task_report_uid;
        std::atomic<size_t> _minute_report_uid;
        size_t _order_report_uid;
        size_t _trade_report_uid;
        size_t _order_num;
        size_t _trade_num;
        size_t _update_order_num;
        size_t _update_trade_num;
        Status _update_status;
        double _progress;
        std::string _remark;
        bool _first_order_flag;
        bool _amount_flag;
        BusinessSideType _businessSideType;
        std::chrono::system_clock::time_point _task_destory_time;
        std::chrono::system_clock::time_point _last_time;
        struct timeval _t_quote;
        std::map<uint64_t, long long> _t_quotes;
        int64_t _maximal_qty;
        ErrorCategory _error_code;
        std::shared_ptr<ats::AssetPositionCalculator> _asset_position_calculator;
        std::map<uint64_t, std::pair<ats::PriceAction, std::shared_ptr<ats::MarketQuotationData>>> _xtp_order_id_market_data;
        std::shared_ptr<ats::MarketData> _current_market_data;
        std::mutex _remark_mutex;
        std::mutex _report_update_mutex;
    };

    inline uint32_t Task::get_task_id() const {
        return _task_id;
    }

    inline size_t Task::get_order_uid() {
        return _trade_report_uid++;
    }

    inline size_t Task::get_trade_uid() {
        return _order_report_uid++;
    }

    inline void Task::set_calculator(std::shared_ptr<ats::AssetPositionCalculator> calculator) {
        _asset_position_calculator = move(calculator);
    }

    inline std::shared_ptr<ats::MarketData> Task::get_current_market_data() const {
        return _current_market_data;
    }

    inline void Task::on_task_report(std::chrono::system_clock::time_point start_time,
                                     std::chrono::system_clock::time_point end_time) {
        on_task_report(start_time, end_time, "");
    }

    inline void Task::task_status_change(uint32_t task_id, ats::Status task_status) {
        Task::task_status_change(task_id, task_status, "");
    }

    inline void Task::on_task_stop() {
        on_task_stop("");
    }


}

#endif //ATS_TASK_H

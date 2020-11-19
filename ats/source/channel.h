//
// Created by Rong Zhou on 2018-08-03.
//

#ifndef ATS_CHANNEL_H
#define ATS_CHANNEL_H

#include <string>
#include "taskreport.h"
#include "orderinfo.h"
#include "tradereport.h"
#include "minutereport.h"
#include "types/errorinfo.h"
#include "task.h"
#include "ordercancelinfo.h"

namespace ats {

    class User;

    class Channel {
    public:
        enum class Command {
            SUBSCRIBE_TASK_REPORT,
            UNSUBSCRIBE_TASK_REPORT,
            GET_TASK_REPORT,
            SUBSCRIBE_ORDER_REPORT,
            UNSUBSCRIBE_ORDER_REPORT,
            GET_ORDER_REPORT,
            SUBSCRIBE_TRADE_REPORT,
            UNSUBSCRIBE_TRADE_REPORT,
            GET_TRADE_REPORT,
            SUBSCRIBE_MINUTE_REPORT,
            UNSUBSCRIBE_MINUTE_REPORT,
            GET_MINUTE_REPORT,
            UNKNOWN
        };

        Command string_to_command(const std::string &str);

        std::string command_to_string(Command command);

    public:
        Channel(std::shared_ptr<User> owner, const std::string &channel_id);

        virtual ~Channel() = default;

    public:
        const std::string &get_channel_id() const;

        virtual void send_user_relogin(const std::string &username) = 0;

        virtual void send_task_id(const std::string &username, uint32_t  task_id, uint64_t task_user_id);

        virtual void
        send_task_status(const std::string &username, Type task_type, uint32_t task_id,
                         std::shared_ptr<TaskReport> task_report, const std::string &user_mac) = 0;

        virtual void send_order_info(const std::string &username, Type task_type, uint32_t task_id,
                                     std::shared_ptr<OrderInfo> order_info,
                                     std::shared_ptr<ErrorInfo> error_info,
                                     size_t order_uid, const std::pair<ats::PriceAction,
                std::shared_ptr<ats::MarketQuotationData>> market_quotation_data) = 0;

        virtual void send_order_cancel_info(const std::string &username, Type task_type, uint32_t task_id,
                                           std::shared_ptr<OrderCancelInfo> order_info,
                                           std::shared_ptr<ErrorInfo> error_info) = 0;


        virtual void
        send_trade_report(const std::string &username, Type task_type, uint32_t task_id,
                          std::shared_ptr<TradeReport> trade_report, size_t trade_uid) = 0;

        virtual void
        send_minute_report(const std::string &username, Type task_type, uint32_t task_id,
                           std::shared_ptr<MinuteReport> minute_report) = 0;

        virtual std::vector<std::string>
        read_minute_report(const std::string &stream, const std::string &username, uint32_t task_id, size_t count,
                           uint32_t start_id, uint32_t end_id) = 0;

        virtual std::vector<std::string>
        read_history_report(const std::string &stream, const std::string &username, uint32_t task_id,
                            size_t count, uint32_t start_id, uint32_t end_id) = 0;

        virtual std::vector<std::string>
        read_last_history_report(const std::string &hash, const std::string &username) = 0;

        virtual uint32_t read_last_task_id() = 0;

    protected:
        std::weak_ptr<User> _owner;
        std::string _channel_id;
        bool _task_report_subscribed;
        bool _order_report_subscribed;
        bool _trade_report_subscribed;
        bool _minute_report_subscribed;
        std::chrono::system_clock::time_point _update_order_time;
        std::chrono::system_clock::time_point _update_trade_time;
        std::chrono::system_clock::time_point _update_task_time;
        std::chrono::system_clock::time_point _update_minute_time;
        uint32_t _subscribed_task_id;
    };

    inline const std::string &Channel::get_channel_id() const {
        return _channel_id;
    }


}

#endif //ATS_CHANNEL_H

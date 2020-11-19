//
// Created by Rong Zhou on 2018-08-03.
//

#include "channel.h"

namespace ats {

    using namespace std;

    Channel::Command Channel::string_to_command(const string &str) {
        if (str == "subscribe_task_report") {
            return Command::SUBSCRIBE_TASK_REPORT;
        }
        if (str == "unsubscribe_task_report") {
            return Command::UNSUBSCRIBE_TASK_REPORT;
        }
        if (str == "get_task_report") {
            return Command::GET_TASK_REPORT;
        }
        if (str == "subscribe_order_report") {
            return Command::SUBSCRIBE_ORDER_REPORT;
        }
        if (str == "unsubscribe_order_report") {
            return Command::UNSUBSCRIBE_ORDER_REPORT;
        }
        if (str == "get_order_report") {
            return Command::GET_ORDER_REPORT;
        }
        if (str == "subscribe_trade_report") {
            return Command::SUBSCRIBE_TRADE_REPORT;
        }
        if (str == "unsubscribe_trade_report") {
            return Command::UNSUBSCRIBE_TRADE_REPORT;
        }
        if (str == "subscribe_minute_report") {
            return Command::SUBSCRIBE_MINUTE_REPORT;
        }
        if (str == "unsubscribe_minute_report") {
            return Command::UNSUBSCRIBE_MINUTE_REPORT;
        }
        if (str == "get_trade_report") {
            return Command::GET_TRADE_REPORT;
        }
        if (str == "get_minute_report") {
            return Command::GET_MINUTE_REPORT;
        }
        return Command::UNKNOWN;
    }

    string Channel::command_to_string(Channel::Command command) {
        switch (command) {
            case Command::SUBSCRIBE_TASK_REPORT:
                return "subscribe_task_report";
            case Command::UNSUBSCRIBE_TASK_REPORT:
                return "unsubscribe_task_report";
            case Command::SUBSCRIBE_ORDER_REPORT:
                return "subscribe_order_report";
            case Command::UNSUBSCRIBE_ORDER_REPORT:
                return "unsubscribe_order_report";
            case Command::GET_ORDER_REPORT:
                return "get_order_report";
            case Command::SUBSCRIBE_TRADE_REPORT:
                return "subscribe_trade_report";
            case Command::UNSUBSCRIBE_TRADE_REPORT:
                return "unsubscribe_trade_report";
            case Command::SUBSCRIBE_MINUTE_REPORT:
                return "subscribe_minute_report";
            case Command::UNSUBSCRIBE_MINUTE_REPORT:
                return "unsubscribe_minute_report";
            case Command::GET_TRADE_REPORT:
                return "get_trade_report";
            case Command::GET_TASK_REPORT:
                return "get_task_report";
            case Command::GET_MINUTE_REPORT:
                return "get_minute_report";
            default:
                return "unknown";
        }
    }

    Channel::Channel(shared_ptr<User> owner, const string &channel_id) : _owner(owner), _channel_id(channel_id) {
        _task_report_subscribed = false;
        _order_report_subscribed = false;
        _trade_report_subscribed = false;
        _minute_report_subscribed = false;
    }

    void Channel::send_task_id(const std::string &username, uint32_t task_id, uint64_t task_user_id) {
        //TODO
    }

}
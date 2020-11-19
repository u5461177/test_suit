//
// Created by Rong Zhou on 2018-08-03.
//

#ifndef ATS_WEBSOCKETCHANNEL_H
#define ATS_WEBSOCKETCHANNEL_H

#include "channel.h"
#include "util/timehelper.h"
#include <chrono>
#include <restbed>

namespace ats {

    class User;

    class WebSocketChannel : public Channel {
    public:
        WebSocketChannel(std::shared_ptr<User> owner, std::shared_ptr<restbed::WebSocket> websocket);

        ~WebSocketChannel() final;

    public:
        void send_user_relogin(const std::string &username);

        void send_task_id(const std::string &username, uint32_t  task_id, uint64_t task_user_id);

        void send_task_status(const std::string &username, Type task_type, uint32_t task_id,
                              std::shared_ptr<TaskReport> task_report, const std::string &user_mac) override;

        void send_order_info(const std::string &username, Type task_type, uint32_t task_id,
                             std::shared_ptr<OrderInfo> order_info,
                             std::shared_ptr<ErrorInfo> error_info,
                             size_t order_uid, const std::pair<ats::PriceAction,
                std::shared_ptr<ats::MarketQuotationData>> market_quotation_data) override;

        void send_order_cancel_info(const std::string &username, Type task_type, uint32_t task_id,
                                    std::shared_ptr<OrderCancelInfo> order_info,
                                    std::shared_ptr<ErrorInfo> error_info) override;

        void
        send_trade_report(const std::string &username, Type task_type, uint32_t task_id,
                          std::shared_ptr<TradeReport> trade_report,
                          size_t trade_uid) override;

        void send_minute_report(const std::string &username, Type task_type, uint32_t task_id,
                                std::shared_ptr<MinuteReport> minute_report) override;

        std::string vector_to_string(const std::vector<std::string> &report);

        std::vector<std::string>
        read_minute_report(const std::string &stream, const std::string &username, uint32_t task_id, size_t count,
                           uint32_t start_id, uint32_t end_id) override;

        std::vector<std::string>
        read_history_report(const std::string &stream, const std::string &username, uint32_t task_id, size_t count,
                            uint32_t start_id, uint32_t end_id) override;

        std::vector<std::string>
        read_last_history_report(const std::string &hash, const std::string &username) override;

        uint32_t read_last_task_id() override;

    private:
        void _subscribed_result_report(const std::string &report_type);

        void _unsubscribed_result_report(const std::string &report_type);

        void on_socket_close(std::shared_ptr<restbed::WebSocket> socket);

        void on_socket_error(std::shared_ptr<restbed::WebSocket> socket, std::error_code error);

        void on_socket_message(std::shared_ptr<restbed::WebSocket> socket,
                               std::shared_ptr<restbed::WebSocketMessage> message);

        std::chrono::system_clock::duration calc_time_span(std::chrono::system_clock::time_point tp) const;

    private:
        std::shared_ptr<restbed::WebSocket> _websocket;
        std::vector<std::string> _task_data;
        std::vector<std::string> _order_data;
        std::vector<std::string> _trade_data;
        std::vector<std::string> _minute_data;
        std::vector<std::string> _relogin_data;
        std::vector<std::string> _task_id_data;
        std::chrono::system_clock::time_point _open_time;
        std::chrono::system_clock::time_point _break_start_time;
        std::chrono::system_clock::time_point _break_end_time;
        std::chrono::system_clock::time_point _close_time;
    };

}

#endif //ATS_WEBSOCKETCHANNEL_H

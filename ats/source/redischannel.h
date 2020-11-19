//
// Created by Rong Zhou on 2018-08-03.
//

#ifndef ATS_REDISCHANNEL_H
#define ATS_REDISCHANNEL_H

#include "channel.h"
#include "json.hpp"

namespace ats {

    class RedisChannel : public Channel {
    public:
        RedisChannel(std::shared_ptr<User> owner);

        ~RedisChannel() override = default;

    public:

        void send_user_relogin(const std::string &username) override;

        void send_task_status(const std::string &username, Type task_type, uint32_t task_id,
                              std::shared_ptr<TaskReport> task_status, const std::string &user_mac) override;

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
                          std::shared_ptr<TradeReport> trade_report, size_t trade_uid) override;

        void send_minute_report(const std::string &username, Type task_type, uint32_t task_id,
                                std::shared_ptr<MinuteReport> minute_report) override;

        std::vector<std::string>
        split(const std::string &str, char delim, std::vector<std::string> &elems, bool skip_empty = true);

        std::vector<uint32_t> read_task_id(const std::string &username);

        uint32_t read_last_task_id();

        std::vector<std::string>
        read_last_history_report(const std::string &hash, const std::string &username) override;

        std::vector<std::string>
        read_minute_report(const std::string &stream, const std::string &username, uint32_t task_id, size_t count,
                           uint32_t start_id, uint32_t end_id) override;

        std::vector<std::string>
        read_history_report(const std::string &stream, const std::string &username, uint32_t task_id,
                            size_t count, uint32_t start_id, uint32_t end_id) override;

    };

}

#endif //ATS_REDISCHANNEL_H

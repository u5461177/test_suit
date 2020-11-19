//
// Created by Rong Zhou on 2018-08-07.
//

#ifndef ATS_MOCKTRADERSERVER_H
#define ATS_MOCKTRADERSERVER_H

#include "singleton.h"
#include "itraderserver.h"

namespace ats {

    class MockTraderServer : public Singleton<MockTraderServer>, ITraderServer {
    public:
        MockTraderServer() = default;

        ~MockTraderServer() final = default;

    public:
        std::pair<std::error_code, uint64_t>
        connect(const std::string &username, const std::string &password, const std::string &server_ip,
                int server_port) override;

        std::error_code disconnect(uint64_t xtp_session_id) override;

        uint64_t place_order(uint32_t task_id, uint64_t xtp_session_id, const Ticker &ticker, const Price &price,
                             const Quantity &quantity, const BusinessType &businessType) override;

        void cancel_order(uint64_t xtp_session_id, uint64_t order_id) override;

        void query_user_asset(uint64_t xtp_session_id) override;

        void query_stock_position(uint64_t xtp_session_id, const std::string &ticker_id) override;

        void query_order_info(uint64_t xtp_session_id, uint64_t order_id) override;

        void query_trade_info(uint64_t xtp_session_id, uint64_t order_id) override;
    };

}

#endif //ATS_MOCKTRADERSERVER_H

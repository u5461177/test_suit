//
// Created by Rong Zhou on 2018-08-03.
//

#ifndef ATS_TRADERSERVER_H
#define ATS_TRADERSERVER_H

#include "singleton.h"
#include "itraderserver.h"
#include "traderspi.h"
#include "types/businesstype.h"

namespace ats {

    class TraderServer : public Singleton<TraderServer>, ITraderServer {
    public:
        TraderServer();

        ~TraderServer() final = default;

    public:
        std::pair<ErrorCategory, uint64_t>
        connect(const std::string &username, const std::string &password, const std::string &server_ip,
                int server_port) override;

        ErrorCategory disconnect(uint64_t xtp_session_id) override;

        uint64_t place_order(uint32_t task_id, uint64_t xtp_session_id, const Ticker &ticker, const Price &price,
                             const Quantity &quantity, const BusinessType &business_type) override;

        void cancel_order(uint64_t xtp_session_id, uint64_t order_id) override;

        void query_user_asset(uint64_t xtp_session_id) override;

        void query_stock_position(uint64_t xtp_session_id, const std::string &ticker_id) override;

        void query_order_info(uint64_t xtp_session_id, uint64_t order_id) override;

        void query_trade_info(uint64_t xtp_session_id, uint64_t order_id) override;

    private:
        XTP::API::TraderApi *_trader_api;
        TraderSPI _trader_spi;
    };

    inline void TraderServer::cancel_order(uint64_t xtp_session_id, uint64_t order_id) {
        _trader_api->CancelOrder(order_id, xtp_session_id);
    }

    inline void TraderServer::query_user_asset(uint64_t xtp_session_id) {
        _trader_api->QueryAsset(xtp_session_id, 0);
    }

    inline void TraderServer::query_stock_position(uint64_t xtp_session_id, const std::string &ticker_id) {
        _trader_api->QueryPosition(ticker_id.c_str(), xtp_session_id, stoi(ticker_id));
    }

    inline void TraderServer::query_order_info(uint64_t xtp_session_id, uint64_t order_id) {
        _trader_api->QueryOrderByXTPID(order_id, xtp_session_id, 0);
    }

    inline void TraderServer::query_trade_info(uint64_t xtp_session_id, uint64_t order_id) {
        _trader_api->QueryTradesByXTPID(order_id, xtp_session_id, 0);
    }

}

#endif //ATS_TRADERSERVER_H

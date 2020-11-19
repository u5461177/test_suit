//
// Created by Rong Zhou on 2018-08-07.
//

#ifndef ATS_ITRADERSERVER_H
#define ATS_ITRADERSERVER_H

#include "types/markettype.h"
#include "types/pricetype.h"
#include "types/sidetype.h"
#include "types/errorcode.h"
#include "types/businesstype.h"

namespace ats {

    using Ticker = std::pair<std::string, MarketType>;
    using Price = std::pair<double, PriceType>;
    using Quantity = std::pair<int64_t, SideType>;

    struct ITraderServer {

        virtual std::pair<ErrorCategory, uint64_t>
        connect(const std::string &username, const std::string &password, const std::string &server_ip,
                int server_port) = 0;

        virtual ErrorCategory disconnect(uint64_t xtp_session_id) = 0;

        virtual uint64_t place_order(uint32_t task_id, uint64_t xtp_session_id, const Ticker &ticker,
                                     const Price &price, const Quantity &quantity,
                                     const BusinessType &businessType) = 0;

        virtual void cancel_order(uint64_t xtp_session_id, uint64_t order_id) = 0;

        virtual void query_user_asset(uint64_t xtp_session_id) = 0;

        virtual void query_stock_position(uint64_t xtp_session_id, const std::string &ticker_id) = 0;

        virtual void query_order_info(uint64_t xtp_session_id, uint64_t order_id) = 0;

        virtual void query_trade_info(uint64_t xtp_session_id, uint64_t order_id) = 0;
    };

}

#endif //ATS_ITRADERSERVER_H

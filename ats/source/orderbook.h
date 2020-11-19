//
// Created by Rong Zhou on 2018-08-03.
//

#ifndef ATS_ORDERBOOK_H
#define ATS_ORDERBOOK_H

#include "types/markettype.h"

namespace ats {

    struct OrderBook {
        OrderBook(XTPOB *orderbook);

        OrderBook(const OrderBook &other);

        MarketType market;
        std::string ticker;
        double last_price;
        int64_t qty;
        double turnover;
        int64_t trades_count;
        double bid[10];
        double ask[10];
        int64_t bid_qty[10];
        int64_t ask_qty[10];
        int64_t data_time;
    };

}

#endif //ATS_ORDERBOOK_H

//
// Created by Rong Zhou on 2018-08-03.
//

#include "orderbook.h"
#include <cstring>

namespace ats {

    OrderBook::OrderBook(XTPOB *orderbook) {
        market = xtp_exchange_type_to_market_type(orderbook->exchange_id);
        ticker = orderbook->ticker;
        last_price = orderbook->last_price;
        qty = orderbook->qty;
        turnover = orderbook->turnover;
        trades_count = orderbook->trades_count;
        memcpy(bid, orderbook->bid, sizeof(double) * 10);
        memcpy(ask, orderbook->ask, sizeof(double) * 10);
        memcpy(bid_qty, orderbook->bid_qty, sizeof(int64_t) * 10);
        memcpy(ask_qty, orderbook->ask_qty, sizeof(int64_t) * 10);
        data_time = orderbook->data_time;
    }

    OrderBook::OrderBook(const OrderBook &other) {
        market = other.market;
        ticker = other.ticker;
        last_price = other.last_price;
        qty = other.qty;
        turnover = other.turnover;
        trades_count = other.trades_count;
        memcpy(bid, other.bid, sizeof(double) * 10);
        memcpy(ask, other.ask, sizeof(double) * 10);
        memcpy(bid_qty, other.bid_qty, sizeof(int64_t) * 10);
        memcpy(ask_qty, other.ask_qty, sizeof(int64_t) * 10);
        data_time = other.data_time;
    }

}
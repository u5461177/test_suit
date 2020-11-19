//
// Created by xhshu on 12/25/18.
//

#include "tickerprice.h"

namespace ats {

    using namespace std;

    TickerPrice::TickerPrice(XTPTPI *ticker_price) {
        ticker = ticker_price->ticker;
        market = xtp_exchange_type_to_market_type(ticker_price->exchange_id);
        last_price = ticker_price->last_price;
    }

    TickerPrice::TickerPrice(const TickerPrice &other) {
        ticker = other.ticker;
        market = other.market;
        last_price = other.last_price;
    }

}
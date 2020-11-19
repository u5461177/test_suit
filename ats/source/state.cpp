//
// Created by galen on 1/14/19.
//

#include "state.h"

namespace ats {

    State::State(const Ticker &ticker, std::shared_ptr<std::vector<std::shared_ptr<MarketData>>> orderbook,
                 int64_t quantity_left, size_t time_left, std::pair<int64_t, double> &qty_price) {
        _ticker = ticker;
        _orderbook = move(orderbook);
        _quantity_left = quantity_left;
        _time_left = time_left;
        _qty_price = qty_price;
    }

}

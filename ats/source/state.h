//
// Created by galen on 1/14/19.
//

#ifndef MUSE_STATE_H
#define MUSE_STATE_H

#include "marketdata.h"
#include <memory>
#include <vector>

namespace ats {
    using Ticker = std::pair<std::string, MarketType>;

    struct State {

        State(const Ticker &ticker, std::shared_ptr<std::vector<std::shared_ptr<MarketData>>> orderbook,
              int64_t quantity_left, size_t time_left, std::pair<int64_t, double> &qty_price);

        Ticker _ticker;
        std::shared_ptr<std::vector<std::shared_ptr<MarketData>>> _orderbook;
        int64_t _quantity_left;
        size_t _time_left;
        std::pair<int64_t, double> _qty_price;
    };

}


#endif //MUSE_STATE_H

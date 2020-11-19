//
// Created by xhshu on 12/25/18.
//

#ifndef ATS_TICKERPRICE_H
#define ATS_TICKERPRICE_H

#include <vector>
#include "types/markettype.h"

namespace ats {

    struct TickerPrice {
        explicit TickerPrice(XTPTPI *ticker_price);

        TickerPrice(const TickerPrice &other);

        std::string ticker;
        MarketType market;
        double last_price;
    };

}


#endif //ATS_TICKERPRICE_H

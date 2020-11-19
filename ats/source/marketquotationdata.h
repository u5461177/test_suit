//
// Created by xtgu on 5/9/19.
//

#ifndef MUSE_MARKETQUOTATIONDATA_H
#define MUSE_MARKETQUOTATIONDATA_H

#include "marketdata.h"
#include "types/pricetype.h"
#include "types/sidetype.h"
#include <memory>

namespace ats {

    struct MarketQuotationData {

        MarketQuotationData(const std::shared_ptr<MarketData > marketData);

        ~MarketQuotationData() = default;

        double _bid[10];
        int64_t _bid_qty[10];
        double _ask[10];
        int64_t _ask_qty[10];

        ats::PriceAction judge_active_passive_price(const SideType &side, double price);
    };
}


#endif //MUSE_MARKETQUOTATIONDATA_H

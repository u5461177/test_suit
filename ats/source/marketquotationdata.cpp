//
// Created by xtgu on 5/9/19.
//

#include <cstring>
#include "marketquotationdata.h"

namespace ats {
    using namespace std;

    MarketQuotationData::MarketQuotationData(const shared_ptr<MarketData> marketData) {
        memcpy(_ask, marketData->ask, sizeof(double) * 10);
        memcpy(_bid, marketData->bid, sizeof(double) * 10);
        memcpy(_ask_qty, marketData->ask_qty, sizeof(int64_t) * 10);
        memcpy(_bid_qty, marketData->bid_qty, sizeof(int64_t) * 10);
    }

    PriceAction MarketQuotationData::judge_active_passive_price(const SideType &side, double price) {
        if (side == SideType::BUY) {
            if (price >= _ask[0]) {
                return PriceAction::ACTIVE;
            } else {
                return PriceAction::PASSIVE;
            }
        } else {
            if (price <= _bid[0]) {
                return PriceAction::ACTIVE;
            } else {
                return PriceAction::PASSIVE;
            }

        }

    }
}
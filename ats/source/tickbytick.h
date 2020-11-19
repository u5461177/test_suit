//
// Created by Rong Zhou on 2018-08-03.
//

#ifndef ATS_TICKBYTICK_H
#define ATS_TICKBYTICK_H

#include "types/markettype.h"

namespace ats {

    struct TickByTick {
        struct TickByTickEntrust {
            int32_t channel_no;
            int64_t seq;
            double price;
            int64_t qty;
            char side;
            char ord_type;
        };

        struct TickByTickTrade {
            int32_t channel_no;
            int64_t seq;
            double price;
            int64_t qty;
            double money;
            int64_t bid_no;
            int64_t ask_no;
            char trade_flag;
        };

        TickByTick(XTPTBT *tick_by_tick);

        TickByTick(const TickByTick &other);

        MarketType market;
        std::string ticker;
        int64_t seq;
        int64_t date_time;
        XTP_TBT_TYPE type;
        union {
            TickByTickEntrust entrust;
            TickByTickTrade trade;
        };
    };

}

#endif //ATS_TICKBYTICK_H

//
// Created by Rong Zhou on 2018-08-03.
//

#ifndef ATS_STOCKPOSITION_H
#define ATS_STOCKPOSITION_H

#include <vector>
#include "types/markettype.h"

namespace ats {

    struct StockPosition {
        static std::vector<std::string> to_record(const StockPosition &stk_position);

        StockPosition(const std::string &ticker_id);

        StockPosition(XTPQueryStkPositionRsp *stk_position);

        StockPosition(const StockPosition &other);

        std::string ticker;
        std::string ticker_name;
        MarketType market;
        int64_t total_qty;
        int64_t sellable_qty;
        double avg_price;
        int64_t yesterday_position;
        int64_t purchase_redeemable_qty;
        XTP_POSITION_DIRECTION_TYPE position_direction;
        int64_t executable_option;
        int64_t lockable_position;
        int64_t locked_position;
        int64_t usable_locked_position;
    };

}

#endif //ATS_STOCKPOSITION_H

//
// Created by Rong Zhou on 2018-07-30.
//

#ifndef ATS_TRADEREPORT_H
#define ATS_TRADEREPORT_H

#include <vector>
#include <json.hpp>
#include "types/markettype.h"
#include "types/sidetype.h"
#include "types/businesstype.h"
#include "types/positioneffecttype.h"
#include "types/ordertype.h"
#include "types/tradetype.h"

namespace ats {

    struct TradeReport {
        static std::vector<std::string> to_record(const TradeReport &trade_report);

        static nlohmann::json to_json(const TradeReport &trade_report);

        TradeReport(XTPTradeReport *trade_report);

        TradeReport(const TradeReport &other);

        uint64_t order_xtp_id;
        uint32_t order_client_id;
        std::string ticker;
        MarketType market;
        uint64_t local_order_id;
        std::string exec_id;
        double price;
        int64_t quantity;
        int64_t trade_time;
        double traded_amount;
        uint64_t report_index;
        std::string order_exch_id;
        TradeTypeType trade_type;
        SideType side;
        PositionEffectType position_effect;
        BusinessType business_type;
        std::string branch_pbu;
    };

}

#endif //ATS_TRADEREPORT_H

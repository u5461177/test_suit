//
// Created by Rong Zhou on 2018-08-03.
//

#ifndef ATS_ORDERINFO_H
#define ATS_ORDERINFO_H

#include <vector>
#include <json.hpp>
#include "types/markettype.h"
#include "types/pricetype.h"
#include "types/sidetype.h"
#include "types/orderstatustype.h"
#include "types/ordersumbitstatustype.h"
#include "types/businesstype.h"
#include "types/positioneffecttype.h"
#include "types/ordertype.h"

namespace ats {

    struct OrderInfo {
        static std::vector<std::string> to_record(const OrderInfo &order_info);

        static nlohmann::json to_json(const OrderInfo &order_info);

        OrderInfo() = default;

        OrderInfo(XTPOrderInfo *order_info);

        OrderInfo(const OrderInfo &other);

        uint64_t order_xtp_id;
        uint32_t order_client_id;
        uint32_t order_cancel_client_id;
        uint64_t order_cancel_xtp_id;
        std::string ticker;
        MarketType market;
        double price;
        int64_t quantity;
        PriceType price_type;
        SideType side;
        PositionEffectType position_effect;
        BusinessType business_type;
        int64_t qty_traded;
        int64_t qty_left;
        int64_t insert_time;
        int64_t update_time;
        int64_t cancel_time;
        double trade_amount;
        std::string order_local_id;
        OrderStatusType order_status;
        OrderSubmitStatusType order_submit_status;
        OrderTypeType order_type;
    };
}

#endif //ATS_ORDERINFO_H

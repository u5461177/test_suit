//
// Created by xhshu on 8/2/18.
//

#include "ordertype.h"

namespace ats {

    using namespace std;

    string txt_order_type_type_to_string(TXTPOrderTypeType order_type) {
        switch (order_type) {
            case XTP_ORDT_Normal:
                return "XTP_ORDT_Normal";
            case XTP_ORDT_DeriveFromQuote:
                return "XTP_ORDT_DeriveFromQuote";
            case XTP_ORDT_DeriveFromCombination:
                return "XTP_ORDT_DeriveFromCombination";
            case XTP_ORDT_Combination:
                return "XTP_ORDT_Combination";
            case XTP_ORDT_ConditionalOrder:
                return "XTP_ORDT_ConditionalOrder";
            case XTP_ORDT_Swap:
                return "XTP_ORDT_Swap";
            default:
                return "UNKNOWN";
        }
    }

    std::string order_type_type_to_string(OrderTypeType order_type) {
        switch (order_type) {
            case OrderTypeType::ORDT_Normal:
                return "ORDT_Normal";
            case OrderTypeType::ORDT_DeriveFromQuote:
                return "ORDT_DeriveFromQuote";
            case OrderTypeType::ORDT_DeriveFromCombination:
                return "ORDT_DeriveFromCombination";
            case OrderTypeType::ORDT_Combination:
                return "ORDT_Combination";
            case OrderTypeType::ORDT_ConditionalOrder:
                return "ORDT_ConditionalOrder";
            case OrderTypeType::ORDT_Swap:
                return "ORDT_Swap";
            default:
                return "UNKNOWN";
        }
    }

    OrderTypeType string_to_order_type_type(const string &str) {
        if (str == "ORDT_Normal")
            return OrderTypeType::ORDT_Normal;
        if (str == "ORDT_DeriveFromQuote")
            return OrderTypeType::ORDT_DeriveFromQuote;
        if (str == "ORDT_DeriveFromCombination")
            return OrderTypeType::ORDT_DeriveFromCombination;
        if (str == "ORDT_Combination")
            return OrderTypeType::ORDT_Combination;
        if (str == "ORDT_ConditionalOrder")
            return OrderTypeType::ORDT_ConditionalOrder;
        if (str == "ORDT_Swap")
            return OrderTypeType::ORDT_Swap;
        return OrderTypeType::ORDT_Normal;
    }

    OrderTypeType txt_order_type_type_to_order_type_type(TXTPOrderTypeType xtp_order_type_type) {
        switch (xtp_order_type_type) {
            case XTP_ORDT_Normal:
                return OrderTypeType::ORDT_Normal;
            case XTP_ORDT_DeriveFromQuote:
                return OrderTypeType::ORDT_DeriveFromQuote;
            case XTP_ORDT_DeriveFromCombination:
                return OrderTypeType::ORDT_DeriveFromCombination;
            case XTP_ORDT_Combination:
                return OrderTypeType::ORDT_Combination;
            case XTP_ORDT_ConditionalOrder:
                return OrderTypeType::ORDT_ConditionalOrder;
            case XTP_ORDT_Swap:
                return OrderTypeType::ORDT_Swap;
            default:
                return OrderTypeType::ORDT_Normal;
        }
    }

    TXTPOrderTypeType order_type_type_to_txt_order_type_type(OrderTypeType order_type) {
        switch (order_type) {
            case OrderTypeType::ORDT_Normal:
                return XTP_ORDT_Normal;
            case OrderTypeType::ORDT_DeriveFromQuote:
                return XTP_ORDT_DeriveFromQuote;
            case OrderTypeType::ORDT_DeriveFromCombination:
                return XTP_ORDT_DeriveFromCombination;
            case OrderTypeType::ORDT_Combination:
                return XTP_ORDT_Combination;
            case OrderTypeType::ORDT_ConditionalOrder:
                return XTP_ORDT_ConditionalOrder;
            case OrderTypeType::ORDT_Swap:
                return XTP_ORDT_Swap;
            default:
                return XTP_ORDT_Normal;
        }
    }

}

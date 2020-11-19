//
// Created by xhshu on 8/2/18.
//

#ifndef ATS_ORDERTYPE_H
#define ATS_ORDERTYPE_H

#include <string>
#include <xtp_api_struct.h>

namespace ats {

    enum class OrderTypeType {
        ORDT_Normal,
        ORDT_DeriveFromQuote,
        ORDT_DeriveFromCombination,
        ORDT_Combination,
        ORDT_ConditionalOrder,
        ORDT_Swap
    };

    std::string txt_order_type_type_to_string(TXTPOrderTypeType order_type);

    std::string order_type_type_to_string(OrderTypeType order_type);

    OrderTypeType string_to_order_type_type(const std::string &str);

    OrderTypeType txt_order_type_type_to_order_type_type(TXTPOrderTypeType order_type);

    TXTPOrderTypeType order_type_type_to_txt_order_type_type(OrderTypeType order_type);
}

#endif //ATS_ORDERTYPE_H

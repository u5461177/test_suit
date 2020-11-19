//
// Created by xhshu on 8/2/18.
//

#ifndef ATS_ORDERSTATUSTYPE_H
#define ATS_ORDERSTATUSTYPE_H

#include <string>
#include <xtp_api_struct.h>

namespace ats {

    enum class OrderStatusType {
        ORDER_STATUS_INIT,
        ORDER_STATUS_ALLTRADED,
        ORDER_STATUS_PARTTRADEDQUEUEING,
        ORDER_STATUS_PARTTRADEDNOTQUEUEING,
        ORDER_STATUS_NOTRADEQUEUEING,
        ORDER_STATUS_CANCELED,
        ORDER_STATUS_REJECTED,
        ORDER_STATUS_UNKNOWN
    };

    OrderStatusType string_to_order_status_type(const std::string &str);

    OrderStatusType xtp_order_status_type_to_order_status_type(XTP_ORDER_STATUS_TYPE order_status_type);

    std::string order_status_type_to_string(OrderStatusType order_status_type);

    std::string xtp_order_status_type_to_string(XTP_ORDER_STATUS_TYPE order_status_type);

    XTP_ORDER_STATUS_TYPE order_status_type_to_xtp_order_status_type(OrderStatusType order_status_type);

}


#endif //ATS_ORDERSTATUSTYPE_H

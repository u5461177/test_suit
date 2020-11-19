//
// Created by xhshu on 8/2/18.
//

#ifndef ATS_ORDERSUMBITSTATUSTYPE_H
#define ATS_ORDERSUMBITSTATUSTYPE_H


#include <string>
#include <xtp_api_struct.h>

namespace ats {

    enum class OrderSubmitStatusType {
        ORDER_SUBMIT_STATUS_INSERT_SUBMITTED,
        ORDER_SUBMIT_STATUS_INSERT_ACCEPTED,
        ORDER_SUBMIT_STATUS_INSERT_REJECTED,
        ORDER_SUBMIT_STATUS_CANCEL_SUBMITTED,
        ORDER_SUBMIT_STATUS_CANCEL_REJECTED,
        ORDER_SUBMIT_STATUS_CANCEL_ACCEPTED,
        ORDER_SUBMIT_STATUS_UNKNOWN
    };

    OrderSubmitStatusType string_to_order_submit_status_type(const std::string &str);

    OrderSubmitStatusType xtp_order_submit_status_type_to_order_subimit_status_type(
            XTP_ORDER_SUBMIT_STATUS_TYPE xtp_order_submit_status_type);

    std::string order_submit_status_type_to_string(OrderSubmitStatusType order_submit_status_type);

    std::string xtp_order_submit_status_type_to_string(XTP_ORDER_SUBMIT_STATUS_TYPE xtp_order_submit_status_type);

    XTP_ORDER_SUBMIT_STATUS_TYPE
    order_submit_status_type_to_xtp_order_submit_status_type(OrderSubmitStatusType order_submit_status_type);


}

#endif //ATS_ORDERSUMBITSTATUSTYPE_H

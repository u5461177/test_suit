//
// Created by xhshu on 8/2/18.
//

#include "ordersumbitstatustype.h"

namespace ats {

    using namespace std;

    OrderSubmitStatusType string_to_order_submit_status_type(const std::string &str) {
        if (str == "ORDER_SUBMIT_STATUS_INSERT_SUBMITTED")
            return OrderSubmitStatusType::ORDER_SUBMIT_STATUS_INSERT_SUBMITTED;
        if (str == "ORDER_SUBMIT_STATUS_INSERT_ACCEPTED")
            return OrderSubmitStatusType::ORDER_SUBMIT_STATUS_INSERT_ACCEPTED;
        if (str == "ORDER_SUBMIT_STATUS_INSERT_REJECTED")
            return OrderSubmitStatusType::ORDER_SUBMIT_STATUS_INSERT_REJECTED;
        if (str == "ORDER_SUBMIT_STATUS_CANCEL_SUBMITTED")
            return OrderSubmitStatusType::ORDER_SUBMIT_STATUS_CANCEL_SUBMITTED;
        if (str == "ORDER_SUBMIT_STATUS_CANCEL_REJECTED")
            return OrderSubmitStatusType::ORDER_SUBMIT_STATUS_CANCEL_REJECTED;
        if (str == "ORDER_SUBMIT_STATUS_CANCEL_ACCEPTED")
            return OrderSubmitStatusType::ORDER_SUBMIT_STATUS_CANCEL_ACCEPTED;
        return OrderSubmitStatusType::ORDER_SUBMIT_STATUS_UNKNOWN;
    }

    string order_submit_status_type_to_string(OrderSubmitStatusType order_submit_status) {
        switch (order_submit_status) {
            case OrderSubmitStatusType::ORDER_SUBMIT_STATUS_INSERT_SUBMITTED:
                return "ORDER_SUBMIT_STATUS_INSERT_SUBMITTED";
            case OrderSubmitStatusType::ORDER_SUBMIT_STATUS_INSERT_ACCEPTED:
                return "ORDER_SUBMIT_STATUS_INSERT_ACCEPTED";
            case OrderSubmitStatusType::ORDER_SUBMIT_STATUS_INSERT_REJECTED:
                return "ORDER_SUBMIT_STATUS_INSERT_REJECTED";
            case OrderSubmitStatusType::ORDER_SUBMIT_STATUS_CANCEL_SUBMITTED:
                return "ORDER_SUBMIT_STATUS_CANCEL_SUBMITTED";
            case OrderSubmitStatusType::ORDER_SUBMIT_STATUS_CANCEL_REJECTED:
                return "ORDER_SUBMIT_STATUS_CANCEL_REJECTED";
            case OrderSubmitStatusType::ORDER_SUBMIT_STATUS_CANCEL_ACCEPTED:
                return "ORDER_SUBMIT_STATUS_CANCEL_ACCEPTED";
            default:
                return "ORDER_SUBMIT_STATUS_UNKNOWN";
        }
    }

    string xtp_order_submit_status_type_to_string(XTP_ORDER_SUBMIT_STATUS_TYPE order_submit_status) {
        switch (order_submit_status) {
            case XTP_ORDER_SUBMIT_STATUS_TYPE::XTP_ORDER_SUBMIT_STATUS_INSERT_SUBMITTED:
                return "ORDER_SUBMIT_STATUS_INSERT_SUBMITTED";
            case XTP_ORDER_SUBMIT_STATUS_TYPE::XTP_ORDER_SUBMIT_STATUS_INSERT_ACCEPTED:
                return "ORDER_SUBMIT_STATUS_INSERT_ACCEPTED";
            case XTP_ORDER_SUBMIT_STATUS_TYPE::XTP_ORDER_SUBMIT_STATUS_INSERT_REJECTED:
                return "ORDER_SUBMIT_STATUS_INSERT_REJECTED";
            case XTP_ORDER_SUBMIT_STATUS_TYPE::XTP_ORDER_SUBMIT_STATUS_CANCEL_SUBMITTED:
                return "ORDER_SUBMIT_STATUS_CANCEL_SUBMITTED";
            case XTP_ORDER_SUBMIT_STATUS_TYPE::XTP_ORDER_SUBMIT_STATUS_CANCEL_REJECTED:
                return "ORDER_SUBMIT_STATUS_CANCEL_REJECTED";
            case XTP_ORDER_SUBMIT_STATUS_TYPE::XTP_ORDER_SUBMIT_STATUS_CANCEL_ACCEPTED:
                return "ORDER_SUBMIT_STATUS_CANCEL_ACCEPTED";
            default:
                return "ORDER_SUBMIT_STATUS_UNKNOWN";
        }
    }

    OrderSubmitStatusType xtp_order_submit_status_type_to_order_subimit_status_type(
            XTP_ORDER_SUBMIT_STATUS_TYPE xtp_order_submit_status_type) {
        switch (xtp_order_submit_status_type) {
            case XTP_ORDER_SUBMIT_STATUS_TYPE::XTP_ORDER_SUBMIT_STATUS_INSERT_SUBMITTED:
                return OrderSubmitStatusType::ORDER_SUBMIT_STATUS_INSERT_SUBMITTED;
            case XTP_ORDER_SUBMIT_STATUS_TYPE::XTP_ORDER_SUBMIT_STATUS_INSERT_ACCEPTED:
                return OrderSubmitStatusType::ORDER_SUBMIT_STATUS_INSERT_ACCEPTED;
            case XTP_ORDER_SUBMIT_STATUS_TYPE::XTP_ORDER_SUBMIT_STATUS_INSERT_REJECTED:
                return OrderSubmitStatusType::ORDER_SUBMIT_STATUS_INSERT_REJECTED;
            case XTP_ORDER_SUBMIT_STATUS_TYPE::XTP_ORDER_SUBMIT_STATUS_CANCEL_SUBMITTED:
                return OrderSubmitStatusType::ORDER_SUBMIT_STATUS_CANCEL_SUBMITTED;
            case XTP_ORDER_SUBMIT_STATUS_TYPE::XTP_ORDER_SUBMIT_STATUS_CANCEL_REJECTED:
                return OrderSubmitStatusType::ORDER_SUBMIT_STATUS_CANCEL_REJECTED;
            case XTP_ORDER_SUBMIT_STATUS_TYPE::XTP_ORDER_SUBMIT_STATUS_CANCEL_ACCEPTED:
                return OrderSubmitStatusType::ORDER_SUBMIT_STATUS_CANCEL_ACCEPTED;
            default:
                return OrderSubmitStatusType::ORDER_SUBMIT_STATUS_UNKNOWN;
        }
    }

    XTP_ORDER_SUBMIT_STATUS_TYPE
    order_submit_status_type_to_xtp_order_submit_status_type(OrderSubmitStatusType order_submit_status) {
        switch (order_submit_status) {
            case OrderSubmitStatusType::ORDER_SUBMIT_STATUS_INSERT_SUBMITTED:
                return XTP_ORDER_SUBMIT_STATUS_TYPE::XTP_ORDER_SUBMIT_STATUS_INSERT_SUBMITTED;
            case OrderSubmitStatusType::ORDER_SUBMIT_STATUS_INSERT_ACCEPTED:
                return XTP_ORDER_SUBMIT_STATUS_TYPE::XTP_ORDER_SUBMIT_STATUS_INSERT_ACCEPTED;
            case OrderSubmitStatusType::ORDER_SUBMIT_STATUS_INSERT_REJECTED:
                return XTP_ORDER_SUBMIT_STATUS_TYPE::XTP_ORDER_SUBMIT_STATUS_INSERT_REJECTED;
            case OrderSubmitStatusType::ORDER_SUBMIT_STATUS_CANCEL_SUBMITTED:
                return XTP_ORDER_SUBMIT_STATUS_TYPE::XTP_ORDER_SUBMIT_STATUS_CANCEL_SUBMITTED;
            case OrderSubmitStatusType::ORDER_SUBMIT_STATUS_CANCEL_REJECTED:
                return XTP_ORDER_SUBMIT_STATUS_TYPE::XTP_ORDER_SUBMIT_STATUS_CANCEL_REJECTED;
            case OrderSubmitStatusType::ORDER_SUBMIT_STATUS_CANCEL_ACCEPTED:
                return XTP_ORDER_SUBMIT_STATUS_TYPE::XTP_ORDER_SUBMIT_STATUS_CANCEL_ACCEPTED;
            default:
                return XTP_ORDER_SUBMIT_STATUS_TYPE::XTP_ORDER_SUBMIT_STATUS_INSERT_SUBMITTED; // TODO
        }
    }

}
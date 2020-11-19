//
// Created by xhshu on 8/2/18.
//

#include "orderstatustype.h"


namespace ats {

    using namespace std;

    OrderStatusType string_to_order_status_type(const string &str) {
        if (str == "ORDER_STATUS_INIT") {
            return OrderStatusType::ORDER_STATUS_INIT;
        }
        if (str == "ORDER_STATUS_ALLTRADED") {
            return OrderStatusType::ORDER_STATUS_ALLTRADED;
        }
        if (str == "ORDER_STATUS_PARTTRADEDQUEUEING") {
            return OrderStatusType::ORDER_STATUS_PARTTRADEDQUEUEING;
        }
        if (str == "ORDER_STATUS_PARTTRADEDNOTQUEUEING") {
            return OrderStatusType::ORDER_STATUS_PARTTRADEDNOTQUEUEING;
        }
        if (str == "ORDER_STATUS_NOTRADEQUEUEING") {
            return OrderStatusType::ORDER_STATUS_NOTRADEQUEUEING;
        }
        if (str == "ORDER_STATUS_CANCELED") {
            return OrderStatusType::ORDER_STATUS_CANCELED;
        }
        if (str == "ORDER_STATUS_REJECTED") {
            return OrderStatusType::ORDER_STATUS_REJECTED;
        }
        return OrderStatusType::ORDER_STATUS_UNKNOWN;
    }

    OrderStatusType xtp_order_status_type_to_order_status_type(XTP_ORDER_STATUS_TYPE order_status_type) {
        switch (order_status_type) {
            case XTP_ORDER_STATUS_INIT:
                return OrderStatusType::ORDER_STATUS_INIT;
            case XTP_ORDER_STATUS_ALLTRADED:
                return OrderStatusType::ORDER_STATUS_ALLTRADED;
            case XTP_ORDER_STATUS_PARTTRADEDQUEUEING:
                return OrderStatusType::ORDER_STATUS_PARTTRADEDQUEUEING;
            case XTP_ORDER_STATUS_PARTTRADEDNOTQUEUEING:
                return OrderStatusType::ORDER_STATUS_PARTTRADEDNOTQUEUEING;
            case XTP_ORDER_STATUS_NOTRADEQUEUEING:
                return OrderStatusType::ORDER_STATUS_NOTRADEQUEUEING;
            case XTP_ORDER_STATUS_CANCELED:
                return OrderStatusType::ORDER_STATUS_CANCELED;
            case XTP_ORDER_STATUS_REJECTED:
                return OrderStatusType::ORDER_STATUS_REJECTED;
            default:
                return OrderStatusType::ORDER_STATUS_UNKNOWN;
        }
    }

    string order_status_type_to_string(OrderStatusType order_status_type) {
        switch (order_status_type) {
            case OrderStatusType::ORDER_STATUS_INIT:
                return "ORDER_STATUS_INIT";
            case OrderStatusType::ORDER_STATUS_ALLTRADED:
                return "ORDER_STATUS_ALLTRADED";
            case OrderStatusType::ORDER_STATUS_PARTTRADEDQUEUEING:
                return "ORDER_STATUS_PARTTRADEDQUEUEING";
            case OrderStatusType::ORDER_STATUS_PARTTRADEDNOTQUEUEING:
                return "ORDER_STATUS_PARTTRADEDNOTQUEUEING";
            case OrderStatusType::ORDER_STATUS_NOTRADEQUEUEING:
                return "ORDER_STATUS_NOTRADEQUEUEING";
            case OrderStatusType::ORDER_STATUS_CANCELED:
                return "ORDER_STATUS_CANCELED";
            case OrderStatusType::ORDER_STATUS_REJECTED:
                return "ORDER_STATUS_REJECTED";
            default:
                return "ORDER_STATUS_UNKNOWN";
        }
    }

    string xtp_order_status_type_to_string(XTP_ORDER_STATUS_TYPE order_status_type) {
        switch (order_status_type) {
            case XTP_ORDER_STATUS_INIT:
                return "ORDER_STATUS_INIT";
            case XTP_ORDER_STATUS_ALLTRADED:
                return "XTP_ORDER_STATUS_ALLTRADED";
            case XTP_ORDER_STATUS_PARTTRADEDQUEUEING:
                return "XTP_ORDER_STATUS_PARTTRADEDQUEUEING";
            case XTP_ORDER_STATUS_PARTTRADEDNOTQUEUEING:
                return "XTP_ORDER_STATUS_PARTTRADEDNOTQUEUEING";
            case XTP_ORDER_STATUS_NOTRADEQUEUEING:
                return "XTP_ORDER_STATUS_NOTRADEQUEUEING";
            case XTP_ORDER_STATUS_CANCELED:
                return "XTP_ORDER_STATUS_CANCELED";
            case XTP_ORDER_STATUS_REJECTED:
                return "XTP_ORDER_STATUS_REJECTED";
            default:
                return "ORDER_STATUS_UNKNOWN";
        }
    }

    XTP_ORDER_STATUS_TYPE order_status_type_to_xtp_order_status_type(OrderStatusType order_status_type) {
        switch (order_status_type) {
            case OrderStatusType::ORDER_STATUS_INIT:
                return XTP_ORDER_STATUS_INIT;
            case OrderStatusType::ORDER_STATUS_ALLTRADED:
                return XTP_ORDER_STATUS_ALLTRADED;
            case OrderStatusType::ORDER_STATUS_PARTTRADEDQUEUEING:
                return XTP_ORDER_STATUS_PARTTRADEDQUEUEING;
            case OrderStatusType::ORDER_STATUS_PARTTRADEDNOTQUEUEING:
                return XTP_ORDER_STATUS_PARTTRADEDNOTQUEUEING;
            case OrderStatusType::ORDER_STATUS_NOTRADEQUEUEING:
                return XTP_ORDER_STATUS_NOTRADEQUEUEING;
            case OrderStatusType::ORDER_STATUS_CANCELED:
                return XTP_ORDER_STATUS_CANCELED;
            case OrderStatusType::ORDER_STATUS_REJECTED:
                return XTP_ORDER_STATUS_REJECTED;
            default:
                return XTP_ORDER_STATUS_UNKNOWN;
        }
    }

}
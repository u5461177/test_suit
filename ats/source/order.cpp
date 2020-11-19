//
// Created by Rong Zhou on 2018-08-03.
//

#include "order.h"
#include "util/gloghelper.h"

namespace ats {

    using namespace std;

    Order::Order(uint64_t xtp_order_id, const Ticker &ticker, const Price &price, const Quantity &quantity)
            : _ticker(ticker), _price(price), _quantity(quantity) {
        _xtp_order_id = xtp_order_id;
    }

    uint64_t Order::get_traded_quantity() {
        uint64_t quantity = 0;
        for (const auto &report : _trade_reports) {
            quantity += report->quantity;
        }
        return quantity;
    }

    double Order::get_traded_amount() {
        double traded_amount = 0.0;
        for (const auto &report : _trade_reports) {
            traded_amount += report->traded_amount;
        }
        return traded_amount;
    }

    double Order::get_repo_traded_ticker_value() {
        double traded_price_amount = 0.0;
        for (const auto &report : _trade_reports) {
            traded_price_amount += (report->quantity * report->price);
        }
        return traded_price_amount;
    }

    size_t Order::get_order_num() {
        uint64_t order_num = 0;
        if (_order_info and (_order_info->order_status == OrderStatusType::ORDER_STATUS_INIT
                             or _order_info->order_status == OrderStatusType::ORDER_STATUS_NOTRADEQUEUEING
                             or _order_info->order_status == OrderStatusType::ORDER_STATUS_ALLTRADED
                             or _order_info->order_status == OrderStatusType::ORDER_STATUS_PARTTRADEDQUEUEING
                             or _order_info->order_status == OrderStatusType::ORDER_STATUS_PARTTRADEDNOTQUEUEING)) {
            order_num++;
        }
        return order_num;
    }

    size_t Order::get_trade_num() {
        uint64_t trade_num = 0;
        if (_order_info and (_order_info->order_status == OrderStatusType::ORDER_STATUS_ALLTRADED or
                             _order_info->order_status == OrderStatusType::ORDER_STATUS_PARTTRADEDNOTQUEUEING)) {
            trade_num++;
        }
        return trade_num;
    }

    int64_t Order::get_left_quantity() {
        // XTP_ORDER_STATUS_PARTTRADEDNOTQUEUEING orders should not be contained in left qty
        if (_order_info and (_order_info->order_status not_eq OrderStatusType::ORDER_STATUS_CANCELED and
                             _order_info->order_status not_eq OrderStatusType::ORDER_STATUS_REJECTED and
                             _order_info->order_status not_eq OrderStatusType::ORDER_STATUS_ALLTRADED and
                             _order_info->order_status not_eq OrderStatusType::ORDER_STATUS_PARTTRADEDNOTQUEUEING)) {
            auto quantity = _quantity.first;
            for (const auto &report : _trade_reports) {
                quantity -= report->quantity;
            }
            return quantity;
        }
        return 0;
    }

    double Order::get_left_amount() {
        // XTP_ORDER_STATUS_PARTTRADEDNOTQUEUEING orders should not be contained in left qty
        if (_order_info and (_order_info->order_status not_eq OrderStatusType::ORDER_STATUS_CANCELED and
                             _order_info->order_status not_eq OrderStatusType::ORDER_STATUS_REJECTED and
                             _order_info->order_status not_eq OrderStatusType::ORDER_STATUS_ALLTRADED and
                             _order_info->order_status not_eq OrderStatusType::ORDER_STATUS_PARTTRADEDNOTQUEUEING)) {
            auto quantity = _quantity.first;
            for (const auto &report : _trade_reports) {
                quantity -= report->quantity;
            }
            if (_order_info->business_type == BusinessType::REPO) {
                return quantity * 100;
            }
            return quantity * _order_info->price;
        }
        return 0.0;
    }

    int64_t Order::get_withdraw_quantity() {
        if (_order_info and (_order_info->order_status == OrderStatusType::ORDER_STATUS_CANCELED
                             or _order_info->order_status == OrderStatusType::ORDER_STATUS_REJECTED
                             or _order_info->order_status == OrderStatusType::ORDER_STATUS_PARTTRADEDNOTQUEUEING)) {
            auto withdraw_qty = _order_info->qty_left;
            return withdraw_qty;
        }
        return 0;
    }

    std::tuple<uint64_t, int64_t, double> Order::get_left_order() {
        if (_order_info and (_order_info->order_status == OrderStatusType::ORDER_STATUS_INIT
                             or _order_info->order_status == OrderStatusType::ORDER_STATUS_NOTRADEQUEUEING
                             or _order_info->order_status == OrderStatusType::ORDER_STATUS_PARTTRADEDQUEUEING
        )) {
            return make_tuple(_order_info->order_xtp_id, _order_info->insert_time, _order_info->price);
        }
        return make_tuple(0, 0, 0);
    }

    string Order::get_error_info() {
        // TODO: all order error info will stop the task
        if (_error_info and _error_info->error_id > 0){
            if(_error_info->error_id ==  11010120 or _error_info->error_id == 11010125) {
                return "";
            }

            return _error_info->error_msg;
        }
        return "";
    }
}
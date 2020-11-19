//
// Created by xtgu on 7/24/19.
//

#include "cashcapitalcalculator.h"
#include "order.h"
#include "glog/logging.h"

namespace ats {
    using namespace std;

    CashCapitalCalculator::CashCapitalCalculator(SideType &sideType) : AssetPositionCalculator(sideType) {

    }

    bool CashCapitalCalculator::update_asset(const std::shared_ptr<ats::OrderInfo> &orderInfo, double &buying_power_add,
                                             double &withdraw_amount) {

        auto side = orderInfo->side;
        auto order_status = orderInfo->order_status;
        double price = orderInfo->price;
        int64_t qty_left = orderInfo->qty_left;
        int64_t quantity = orderInfo->quantity;
        int64_t qty_traded = orderInfo->qty_traded;

        if (side == SideType::BUY) {
            switch (order_status) {
                case OrderStatusType::ORDER_STATUS_INIT:
                    buying_power_add = 0;
                    withdraw_amount = 0;
                    break;
                case OrderStatusType::ORDER_STATUS_NOTRADEQUEUEING:
                    buying_power_add = 0;
                    withdraw_amount = 0;
                    break;
                case OrderStatusType::ORDER_STATUS_ALLTRADED:
                    buying_power_add = 0;
                    withdraw_amount = 0;
                    break;
                case OrderStatusType::ORDER_STATUS_PARTTRADEDQUEUEING:
                    buying_power_add = 0;
                    withdraw_amount = 0;
                    break;
                case OrderStatusType::ORDER_STATUS_PARTTRADEDNOTQUEUEING:
                    buying_power_add = qty_left * price + (amount_fee(quantity*price) - amount_fee(qty_traded*price));
                    withdraw_amount = buying_power_add;
                    break;
                case OrderStatusType::ORDER_STATUS_CANCELED:
                    buying_power_add = qty_left * price + amount_fee(qty_left*price);
                    withdraw_amount = buying_power_add;
                    break;
                case OrderStatusType::ORDER_STATUS_REJECTED:
                    buying_power_add = quantity * price + amount_fee(quantity*price);
                    withdraw_amount = buying_power_add;
                    break;
                default:
                    LOG(ERROR) << "Get unknow order status of order | " << orderInfo->order_xtp_id;
            }

        } else if (side == SideType::SELL) {
            switch (order_status) {
                case OrderStatusType::ORDER_STATUS_INIT:
                    buying_power_add = 0;
                    withdraw_amount = 0;
                    break;
                case OrderStatusType::ORDER_STATUS_NOTRADEQUEUEING:
                    buying_power_add = 0;
                    withdraw_amount = 0;
                    break;
                case OrderStatusType::ORDER_STATUS_ALLTRADED:
                    buying_power_add = qty_traded * price - amount_fee(qty_traded*price);
                    withdraw_amount = 0;
                    break;
                case OrderStatusType::ORDER_STATUS_PARTTRADEDQUEUEING:
                    buying_power_add = 0;
                    withdraw_amount = 0;
                    break;
                case OrderStatusType::ORDER_STATUS_PARTTRADEDNOTQUEUEING:
                    buying_power_add = qty_traded * price - amount_fee(qty_traded * price);
                    withdraw_amount = qty_left * price;
                    break;
                case OrderStatusType::ORDER_STATUS_CANCELED:
                    buying_power_add = 0;
                    withdraw_amount = quantity * price;
                    break;
                case OrderStatusType::ORDER_STATUS_REJECTED:
                    buying_power_add = 0;
                    withdraw_amount = quantity * price;
                    break;
                default:
                    LOG(ERROR) << "Get unknow order status of order | " << orderInfo->order_xtp_id;
            }

        } else {
            throw ErrorCategory(ErrorCode::INVALID_SIDE_TYPE_PARAMETER);
        }
        return true;
    }


    bool CashCapitalCalculator::update_position(int64_t qty_left, ats::SideType side, int64_t &result) {
        int64_t sellable_quantity = 0;
        if (side == SideType::SELL) {
            sellable_quantity = qty_left;
        }
        result = sellable_quantity;
        return true;
    }


    double CashCapitalCalculator::amount_with_fee(double traded_amount) {
        if (_sideType == SideType::BUY) {
            return (traded_amount + amount_fee(traded_amount));

        } else {
            return (traded_amount - amount_fee(traded_amount));
        }

    }

    double CashCapitalCalculator::amount_fee(double traded_amount) {
        if (_sideType == SideType::BUY) {
            return max(_min_trade_cost, traded_amount * _tax_rate);

        } else {
            return (max(_min_trade_cost, traded_amount * _tax_rate) + traded_amount * _stamp_tax_rate);

        }

    }

    int64_t CashCapitalCalculator::update_quantity_with_amount(Quantity &quantity, double &ticker_trade_price,
                                                               double left_amount, int64_t placed_qty, double price) {
        int64_t current_quantity = 0;
        int64_t left_quantity = 0;
        ticker_trade_price = price;

        if (quantity.second == SideType::SELL) {
            left_quantity = (int64_t) (ceil(((left_amount + max(_min_trade_cost, left_amount * _tax_rate)) /
                                             ticker_trade_price) / _minimal_qty) * _minimal_qty);
            current_quantity = left_quantity + placed_qty;

        } else {
            left_quantity = (int64_t) (floor(((left_amount - max(_min_trade_cost, left_amount * _tax_rate)) /
                                              ticker_trade_price) / _minimal_qty) * _minimal_qty);
            current_quantity = left_quantity + placed_qty;
        }

        return current_quantity;
    }


}

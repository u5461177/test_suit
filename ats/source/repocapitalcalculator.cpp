//
// Created by xtgu on 7/24/19.
//

#include "repocapitalcalculator.h"


namespace ats {
    using namespace std;

    RepoCapitalCalculator::RepoCapitalCalculator(SideType &sideType, MarketType &marketType) : AssetPositionCalculator(
            sideType) {
        if (marketType == MarketType::SZ) {
            _minimal_qty = 10;

        } else if (marketType == MarketType::SH) {
            _minimal_qty = 1000;

        } else {

            throw ErrorCategory(ErrorCode::WRONG_MARKET_TYPE);
        }
    }


    bool RepoCapitalCalculator::update_asset(const std::shared_ptr<OrderInfo> &orderInfo, double &buying_power_add,
                                             double &withdraw_amount) {
        auto side = orderInfo->side;
        auto order_status = orderInfo->order_status;
        double price = orderInfo->price;
        int64_t qty_left = orderInfo->qty_left;
        int64_t quantity = orderInfo->quantity;
        int64_t qty_traded = orderInfo->qty_traded;

        if (side == SideType::SELL) {
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
                    buying_power_add = qty_left * price + amount_fee(quantity*price) - amount_fee(qty_traded*price);
                    withdraw_amount = buying_power_add;
                    break;
                case OrderStatusType::ORDER_STATUS_CANCELED:
                    buying_power_add = quantity * price + amount_fee(quantity*price);
                    withdraw_amount = buying_power_add;
                    break;
                case OrderStatusType::ORDER_STATUS_REJECTED:
                    buying_power_add = quantity * price + amount_fee(quantity*price);
                    withdraw_amount = buying_power_add;
                    break;
                default:
                    LOG(ERROR) << "Get unknow order status of order | " << orderInfo->order_xtp_id;
            }
        } else {
            throw ErrorCategory(ErrorCode::INVALID_SIDE_TYPE_PARAMETER);
        }
        return true;
    }

    bool RepoCapitalCalculator::update_position(int64_t qty_left, SideType side, int64_t &result) {
        result = 0;
        return true;
    }

    double RepoCapitalCalculator::amount_fee(double traded_amount) {
        return (max(_min_trade_cost, traded_amount * _tax_rate));
    }

    double RepoCapitalCalculator::amount_with_fee(double traded_amount) {
        return traded_amount + amount_fee(traded_amount);
    }

    int64_t RepoCapitalCalculator::update_quantity_with_amount(Quantity &quantity, double &ticker_trade_price,
                                                               double left_amount, int64_t placed_qty, double price) {

        int64_t current_quantity = 0;
        int64_t left_quantity = 0;
        ticker_trade_price = _repo_price;

        left_quantity = (int64_t) (
                floor(((left_amount - max(_min_trade_cost, left_amount * _tax_rate)) / ticker_trade_price) /
                      _minimal_qty) * _minimal_qty);
        current_quantity = left_quantity + placed_qty;
        return current_quantity;
    }

    double RepoCapitalCalculator::get_correct_price(double price) {
        return _repo_price;
    }

}

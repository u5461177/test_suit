//
// Created by Rong Zhou on 2018-08-03.
//

#ifndef ATS_ORDER_H
#define ATS_ORDER_H

#include <vector>
#include <string>
#include <iostream>
#include "orderinfo.h"
#include "types/errorinfo.h"
#include "tradereport.h"

namespace ats {

    using Ticker = std::pair<std::string, MarketType>;
    using Price = std::pair<double, PriceType>;
    using Quantity = std::pair<int64_t, SideType>;

    class Order {
    public:
        Order(uint64_t xtp_order_id, const Ticker &ticker, const Price &price, const Quantity &quantity);

        virtual ~Order() = default;

    public:
        uint64_t get_xtp_order_id() const;

        const Quantity &get_quantity() const;

        const Price &get_price() const;

        uint64_t get_traded_quantity();

        double get_traded_amount();

        double get_repo_traded_ticker_value();

        int64_t get_left_quantity();

        int64_t get_withdraw_quantity();

        double get_left_amount();

        size_t get_order_num();

        size_t get_trade_num();

        std::tuple<uint64_t, int64_t, double> get_left_order();

        std::string get_error_info();

        void on_order_info(std::shared_ptr<OrderInfo> order_info, std::shared_ptr<ErrorInfo> error_info);

        void on_trade_report(std::shared_ptr<TradeReport> trade_report);

    private:
        uint64_t _xtp_order_id;
        Ticker _ticker;
        Price _price;
        Quantity _quantity;
        std::shared_ptr<OrderInfo> _order_info;
        std::shared_ptr<ErrorInfo> _error_info;
        std::vector<std::shared_ptr<TradeReport>> _trade_reports;
    };

    inline uint64_t Order::get_xtp_order_id() const {
        return _xtp_order_id;
    }

    inline const Price &Order::get_price() const {
        return _price;
    }

    inline const Quantity &Order::get_quantity() const {
        return _quantity;
    }

    inline void Order::on_order_info(std::shared_ptr<OrderInfo> order_info, std::shared_ptr<ErrorInfo> error_info) {
        _order_info = move(order_info);
        _error_info = move(error_info);
    }

    inline void Order::on_trade_report(std::shared_ptr<TradeReport> trade_report) {
        _trade_reports.push_back(move(trade_report));
    }

}

#endif //ATS_ORDER_H

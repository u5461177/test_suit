//
// Created by Rong Zhou on 2018-08-03.
//

#ifndef ATS_MARKETDATA_H
#define ATS_MARKETDATA_H

#include "types/markettype.h"

namespace ats {
    struct MarketData {
        struct StockMarketData {
            int64_t total_bid_qty;
            int64_t total_ask_qty;
            double ma_bid_price;
            double ma_ask_price;
            double ma_bond_bid_price;
            double ma_bond_ask_price;
            double yield_to_maturity;
            double iopv;
            int32_t etf_buy_count;
            int32_t etf_sell_count;
            double etf_buy_qty;
            double etf_buy_money;
            double etf_sell_qty;
            double etf_sell_money;
            double total_warrant_exec_qty;
            double warrant_lower_price;
            double warrant_upper_price;
            int32_t cancel_buy_count;
            int32_t cancel_sell_count;
            double cancel_buy_qty;
            double cancel_sell_qty;
            double cancel_buy_money;
            double cancel_sell_money;
            int64_t total_buy_count;
            int64_t total_sell_count;
            int32_t duration_after_buy;
            int32_t duration_after_sell;
            int32_t num_bid_orders;
            int32_t num_ask_orders;
            double pre_iopv;
        };

        struct OptionMarketData {
            double auction_price;
            int64_t auction_qty;
            int64_t last_enquiry_time;
        };

        explicit MarketData() = default;

        explicit MarketData(XTPMD *market_data);

        MarketData(const MarketData &other);

        MarketType market;
        std::string ticker;
        double last_price;
        double pre_close_price;
        double open_price;
        double high_price;
        double low_price;
        double close_price;
        int64_t pre_total_long_position;
        int64_t total_long_position;
        double pre_settl_price;
        double settl_price;
        double upper_limit_price;
        double lower_limit_price;
        double pre_delta;
        double curr_delta;
        int64_t data_time;
        int64_t qty;
        double turnover;
        double avg_price;
        double bid[10];
        double ask[10];
        int64_t bid_qty[10];
        int64_t ask_qty[10];
        int64_t trades_count;
        std::string ticker_status;
        XTP_MARKETDATA_TYPE data_type;
        union {
            StockMarketData stk;
            OptionMarketData opt;
        };
    };

}

#endif //ATS_MARKETDATA_H

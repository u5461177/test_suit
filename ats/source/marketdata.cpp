//
// Created by Rong Zhou on 2018-08-03.
//

#include "marketdata.h"
#include <cstring>

namespace ats {

    MarketData::MarketData(XTPMD *market_data) {
        market = xtp_exchange_type_to_market_type(market_data->exchange_id);
        ticker = market_data->ticker;
        last_price = market_data->last_price;
        pre_close_price = market_data->pre_close_price;
        open_price = market_data->open_price;
        high_price = market_data->high_price;
        low_price = market_data->low_price;
        close_price = market_data->close_price;
        pre_total_long_position = market_data->pre_total_long_positon;
        total_long_position = market_data->total_long_positon;
        pre_settl_price = market_data->pre_settl_price;
        settl_price = market_data->settl_price;
        upper_limit_price = market_data->upper_limit_price;
        lower_limit_price = market_data->lower_limit_price;
        pre_delta = market_data->pre_delta;
        curr_delta = market_data->curr_delta;
        data_time = market_data->data_time;
        qty = market_data->qty;
        turnover = market_data->turnover;
        avg_price = market_data->avg_price;
        trades_count = market_data->trades_count;
        ticker_status = market_data->ticker_status;
        data_type = market_data->data_type;
        memcpy(bid, market_data->bid, sizeof(double) * 10);
        memcpy(ask, market_data->ask, sizeof(double) * 10);
        memcpy(bid_qty, market_data->bid_qty, sizeof(int64_t) * 10);
        memcpy(ask_qty, market_data->ask_qty, sizeof(int64_t) * 10);
        if (data_type == XTP_MARKETDATA_ACTUAL) {
            stk.total_bid_qty = market_data->stk.total_bid_qty;
            stk.total_ask_qty = market_data->stk.total_ask_qty;
            stk.ma_bid_price = market_data->stk.ma_bid_price;
            stk.ma_ask_price = market_data->stk.ma_ask_price;
            stk.ma_bond_bid_price = market_data->stk.ma_bond_bid_price;
            stk.ma_bond_ask_price = market_data->stk.ma_bond_ask_price;
            stk.yield_to_maturity = market_data->stk.yield_to_maturity;
            stk.iopv = market_data->stk.iopv;
            stk.etf_buy_count = market_data->stk.etf_buy_count;
            stk.etf_sell_count = market_data->stk.etf_sell_count;
            stk.etf_buy_qty = market_data->stk.etf_buy_qty;
            stk.etf_buy_money = market_data->stk.etf_buy_money;
            stk.etf_sell_qty = market_data->stk.etf_sell_qty;
            stk.etf_sell_money = market_data->stk.etf_sell_money;
            stk.total_warrant_exec_qty = market_data->stk.total_warrant_exec_qty;
            stk.warrant_lower_price = market_data->stk.warrant_lower_price;
            stk.warrant_upper_price = market_data->stk.warrant_upper_price;
            stk.cancel_buy_count = market_data->stk.cancel_buy_count;
            stk.cancel_sell_count = market_data->stk.cancel_sell_count;
            stk.cancel_buy_qty = market_data->stk.cancel_buy_qty;
            stk.cancel_sell_qty = market_data->stk.cancel_sell_qty;
            stk.cancel_buy_money = market_data->stk.cancel_buy_money;
            stk.cancel_sell_money = market_data->stk.cancel_sell_money;
            stk.total_buy_count = market_data->stk.total_buy_count;
            stk.total_sell_count = market_data->stk.total_sell_count;
            stk.duration_after_buy = market_data->stk.duration_after_buy;
            stk.duration_after_sell = market_data->stk.duration_after_sell;
            stk.num_bid_orders = market_data->stk.num_bid_orders;
            stk.num_ask_orders = market_data->stk.num_ask_orders;
            stk.pre_iopv = market_data->stk.pre_iopv;
        } else if (data_type == XTP_MARKETDATA_OPTION) {
            opt.auction_price = market_data->opt.auction_price;
            opt.auction_qty = market_data->opt.auction_qty;
            opt.last_enquiry_time = market_data->opt.last_enquiry_time;
        }
    }

    MarketData::MarketData(const MarketData &other) {
        market = other.market;
        ticker = other.ticker;
        last_price = other.last_price;
        pre_close_price = other.pre_close_price;
        open_price = other.open_price;
        high_price = other.high_price;
        low_price = other.low_price;
        close_price = other.close_price;
        pre_total_long_position = other.pre_total_long_position;
        total_long_position = other.total_long_position;
        pre_settl_price = other.pre_settl_price;
        settl_price = other.settl_price;
        upper_limit_price = other.upper_limit_price;
        lower_limit_price = other.lower_limit_price;
        pre_delta = other.pre_delta;
        curr_delta = other.curr_delta;
        data_time = other.data_time;
        qty = other.qty;
        turnover = other.turnover;
        avg_price = other.avg_price;
        trades_count = other.trades_count;
        ticker_status = other.ticker_status;
        data_type = other.data_type;
        memcpy(bid, other.bid, sizeof(double) * 10);
        memcpy(ask, other.ask, sizeof(double) * 10);
        memcpy(bid_qty, other.bid_qty, sizeof(int64_t) * 10);
        memcpy(ask_qty, other.ask_qty, sizeof(int64_t) * 10);
        if (data_type == XTP_MARKETDATA_ACTUAL) {
            stk.total_bid_qty = other.stk.total_bid_qty;
            stk.total_ask_qty = other.stk.total_ask_qty;
            stk.ma_bid_price = other.stk.ma_bid_price;
            stk.ma_ask_price = other.stk.ma_ask_price;
            stk.ma_bond_bid_price = other.stk.ma_bond_bid_price;
            stk.ma_bond_ask_price = other.stk.ma_bond_ask_price;
            stk.yield_to_maturity = other.stk.yield_to_maturity;
            stk.iopv = other.stk.iopv;
            stk.etf_buy_count = other.stk.etf_buy_count;
            stk.etf_sell_count = other.stk.etf_sell_count;
            stk.etf_buy_qty = other.stk.etf_buy_qty;
            stk.etf_buy_money = other.stk.etf_buy_money;
            stk.etf_sell_qty = other.stk.etf_sell_qty;
            stk.etf_sell_money = other.stk.etf_sell_money;
            stk.total_warrant_exec_qty = other.stk.total_warrant_exec_qty;
            stk.warrant_lower_price = other.stk.warrant_lower_price;
            stk.warrant_upper_price = other.stk.warrant_upper_price;
            stk.cancel_buy_count = other.stk.cancel_buy_count;
            stk.cancel_sell_count = other.stk.cancel_sell_count;
            stk.cancel_buy_qty = other.stk.cancel_buy_qty;
            stk.cancel_sell_qty = other.stk.cancel_sell_qty;
            stk.cancel_buy_money = other.stk.cancel_buy_money;
            stk.cancel_sell_money = other.stk.cancel_sell_money;
            stk.total_buy_count = other.stk.total_buy_count;
            stk.total_sell_count = other.stk.total_sell_count;
            stk.duration_after_buy = other.stk.duration_after_buy;
            stk.duration_after_sell = other.stk.duration_after_sell;
            stk.num_bid_orders = other.stk.num_bid_orders;
            stk.num_ask_orders = other.stk.num_ask_orders;
            stk.pre_iopv = other.stk.pre_iopv;
        } else if (data_type == XTP_MARKETDATA_OPTION) {
            opt.auction_price = other.opt.auction_price;
            opt.auction_qty = other.opt.auction_qty;
            opt.last_enquiry_time = other.opt.last_enquiry_time;
        }
    }

}
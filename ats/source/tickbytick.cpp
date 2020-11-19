//
// Created by Rong Zhou on 2018-08-03.
//

#include "tickbytick.h"

namespace ats {

    TickByTick::TickByTick(XTPTBT *tick_by_tick) {
        market = xtp_exchange_type_to_market_type(tick_by_tick->exchange_id);
        ticker = tick_by_tick->ticker;
        seq = tick_by_tick->seq;
        date_time = tick_by_tick->data_time;
        type = tick_by_tick->type;
        if (market == MarketType::SZ) {
            entrust.channel_no = tick_by_tick->entrust.channel_no;
            entrust.seq = tick_by_tick->entrust.seq;
            entrust.price = tick_by_tick->entrust.price;
            entrust.qty = tick_by_tick->entrust.qty;
            entrust.side = tick_by_tick->entrust.side;
            entrust.ord_type = tick_by_tick->entrust.ord_type;
        } else if (market == MarketType::SH) {
            trade.channel_no = tick_by_tick->trade.channel_no;
            trade.seq = tick_by_tick->trade.seq;
            trade.price = tick_by_tick->trade.price;
            trade.qty = tick_by_tick->trade.qty;
            trade.money = tick_by_tick->trade.money;
            trade.bid_no = tick_by_tick->trade.bid_no;
            trade.ask_no = tick_by_tick->trade.ask_no;
            trade.trade_flag = tick_by_tick->trade.trade_flag;
        }
    }

    TickByTick::TickByTick(const TickByTick &other) {
        market = other.market;
        ticker = other.ticker;
        seq = other.seq;
        date_time = other.date_time;
        type = other.type;
        if (market == MarketType::SZ) {
            entrust.channel_no = other.entrust.channel_no;
            entrust.seq = other.entrust.seq;
            entrust.price = other.entrust.price;
            entrust.qty = other.entrust.qty;
            entrust.side = other.entrust.side;
            entrust.ord_type = other.entrust.ord_type;
        } else if (market == MarketType::SH) {
            trade.channel_no = other.trade.channel_no;
            trade.seq = other.trade.seq;
            trade.price = other.trade.price;
            trade.qty = other.trade.qty;
            trade.money = other.trade.money;
            trade.bid_no = other.trade.bid_no;
            trade.ask_no = other.trade.ask_no;
            trade.trade_flag = other.trade.trade_flag;
        }
    }

}
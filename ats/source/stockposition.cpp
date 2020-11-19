//
// Created by Rong Zhou on 2018-08-03.
//

#include "stockposition.h"

namespace ats {

    using namespace std;

    vector<string> StockPosition::to_record(const StockPosition &stk_position) {
        return {};
    }

    StockPosition::StockPosition(const string &ticker_id) {
        ticker = ticker_id;
        ticker_name = "";
        market = MarketType::UNKNOWN;
        total_qty = 0;
        sellable_qty = 0;
        avg_price = 0.0;
        yesterday_position = 0;
        purchase_redeemable_qty = 0;
        position_direction = XTP_POSITION_DIRECTION_NET;
        executable_option = 0;
        lockable_position = 0;
        locked_position = 0;
        usable_locked_position = 0;
    }

    StockPosition::StockPosition(XTPQueryStkPositionRsp *stk_position) {
        ticker = stk_position->ticker;
        ticker_name = stk_position->ticker_name;
        market = xtp_market_type_to_market_type(stk_position->market);
        total_qty = stk_position->total_qty;
        sellable_qty = stk_position->sellable_qty;
        avg_price = stk_position->avg_price;
        yesterday_position = stk_position->yesterday_position;
        purchase_redeemable_qty = stk_position->purchase_redeemable_qty;
        position_direction = stk_position->position_direction;
        executable_option = stk_position->executable_option;
        lockable_position = stk_position->lockable_position;
        locked_position = stk_position->locked_position;
        usable_locked_position = stk_position->usable_locked_position;
    }

    StockPosition::StockPosition(const StockPosition &other) {
        ticker = other.ticker;
        ticker_name = other.ticker_name;
        market = other.market;
        total_qty = other.total_qty;
        sellable_qty = other.sellable_qty;
        avg_price = other.avg_price;
        yesterday_position = other.yesterday_position;
        purchase_redeemable_qty = other.purchase_redeemable_qty;
        position_direction = other.position_direction;
        executable_option = other.executable_option;
        lockable_position = other.lockable_position;
        locked_position = other.locked_position;
        usable_locked_position = other.usable_locked_position;
    }

}
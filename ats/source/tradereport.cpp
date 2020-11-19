//
// Created by Rong Zhou on 2018-07-30.
//

#include "tradereport.h"

namespace ats {

    using namespace std;
    using json = nlohmann::json;

    vector<string> TradeReport::to_record(const TradeReport &trade_report) {
        static const string report_type_("report_type");
        static const string task_type_("task_type");
        static const string order_xtp_id_("order_xtp_id");
        static const string order_client_id_("order_client_id");
        static const string ticker_("ticker");
        static const string market_("market");
        static const string local_order_id_("local_order_id");
        static const string exec_id_("exec_id");
        static const string price_("price");
        static const string quantity_("quantity");
        static const string trade_time_("trade_time");
        static const string traded_amount_("traded_amount");
        static const string report_index_("report_index");
        static const string order_exch_id_("order_exch_id");
        static const string trade_type_("trade_type");
        static const string side_("side");
        static const string position_effect_("position_effect");
        static const string business_type_("business_type");
        static const string branch_pbu_("branch_pbu");
        auto report_type = "trade_report";
//        auto task_type = type_to_string(trade_report.task_type);
        auto order_xtp_id = to_string(trade_report.order_xtp_id);
        auto order_client_id = to_string(trade_report.order_client_id);
        auto ticker = trade_report.ticker;
        auto market = market_type_to_string(trade_report.market);
        auto local_order_id = to_string(trade_report.local_order_id);
        auto exec_id = trade_report.exec_id;
        auto price = to_string(trade_report.price);
        auto quantity = to_string(trade_report.quantity);
        auto trade_time = to_string(trade_report.trade_time);
        auto traded_amount = to_string(trade_report.traded_amount);
        auto report_index = to_string(trade_report.report_index);
        auto order_exch_id = trade_report.order_exch_id;
        auto trade_type = trade_type_type_to_string(trade_report.trade_type);
        auto side = side_type_to_string(trade_report.side);
        auto position_effect = position_effect_type_to_string(trade_report.position_effect);
        auto business_type = business_type_to_string(trade_report.business_type);
        auto branch_pbu = trade_report.branch_pbu;

        return vector<string>{report_type_, report_type, order_xtp_id_, order_xtp_id, order_client_id_, order_client_id,
                              ticker_, ticker, market_, market, local_order_id_, local_order_id, exec_id_, exec_id,
                              price_, price, quantity_, quantity, trade_time_, trade_time, traded_amount_,
                              traded_amount,
                              report_index_, report_index, order_exch_id_, order_exch_id, trade_type_, trade_type,
                              side_, side, position_effect_, position_effect, business_type_, business_type,
                              branch_pbu_, branch_pbu};
    }

    TradeReport::TradeReport(XTPTradeReport *trade_report) {
        order_xtp_id = trade_report->order_xtp_id;
        order_client_id = trade_report->order_client_id;
        ticker = trade_report->ticker;
        market = xtp_market_type_to_market_type(trade_report->market);
        local_order_id = trade_report->local_order_id;
        exec_id = trade_report->exec_id;
        price = trade_report->price;
        quantity = trade_report->quantity;
        trade_time = trade_report->trade_time;
        traded_amount = trade_report->trade_amount;
        report_index = trade_report->report_index;
        order_exch_id = trade_report->order_exch_id;
        trade_type = txtp_trade_type_type_to_trade_type_type(trade_report->trade_type);
        side = xtp_side_type_to_side_type(trade_report->side);
        position_effect = xtp_position_effect_type_to_position_effect_type(trade_report->position_effect);
        business_type = xtp_business_type_to_business_type(trade_report->business_type);
        branch_pbu = trade_report->branch_pbu;
    }

    TradeReport::TradeReport(const TradeReport &other) {
        order_xtp_id = other.order_xtp_id;
        order_client_id = other.order_client_id;
        ticker = other.ticker;
        market = other.market;
        local_order_id = other.local_order_id;
        exec_id = other.exec_id;
        price = other.price;
        quantity = other.quantity;
        trade_time = other.trade_time;
        traded_amount = other.traded_amount;
        report_index = other.report_index;
        order_exch_id = other.order_exch_id;
        trade_type = other.trade_type;
        side = other.side;
        position_effect = other.position_effect;
        business_type = other.business_type;
        branch_pbu = other.branch_pbu;
    }

    json TradeReport::to_json(const ats::TradeReport &report) {
        json result;
        result["report_type"] = "trade_report";
        result["order_xtp_id"] = report.order_xtp_id;
        result["order_client_id"] = report.order_client_id;
        result["ticker"] = report.ticker;
        result["market"] = report.market;
        result["local_order_id"] = report.local_order_id;
        result["exec_id"] = report.exec_id;
        result["price"] = report.price;
        result["quantity"] = report.quantity;
        result["trade_time"] = report.trade_time;
        result["traded_amount"] = report.traded_amount;
        result["report_index"] = report.report_index;
        result["order_exch_id"] = report.order_exch_id;
        result["trade_type"] = trade_type_type_to_string(report.trade_type);
        result["side"] = side_type_to_string(report.side);
        result["position_effect"] = position_effect_type_to_string(report.position_effect);
        result["business_type"] = business_type_to_string(report.business_type);
        result["branch_pbu"] = report.branch_pbu;
        return result;
    }

}
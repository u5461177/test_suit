//
// Created by Rong Zhou on 2018-07-30.
//

#include "orderinfo.h"

namespace ats {

    using namespace std;
    using json = nlohmann::json;

    vector<string> OrderInfo::to_record(const OrderInfo &order_info) {
        static const string report_type_("report_type");
        static const string task_type_("task_type");
        static const string order_xtp_id_("order_xtp_id");
        static const string order_client_id_("order_client_id");
        static const string order_cancel_client_id_("order_cancel_client_id");
        static const string order_cancel_xtp_id_("order_cancel_xtp_id");
        static const string ticker_("ticker");
        static const string market_("market");
        static const string price_("price");
        static const string quantity_("quantity");
        static const string price_type_("price_type");
        static const string side_("side");
        static const string position_effect_("position_effect");
        static const string business_type_("business_type");
        static const string qty_traded_("qty_traded");
        static const string qty_left_("qty_left");
        static const string insert_time_("insert_time");
        static const string update_time_("update_time");
        static const string cancel_time_("cancel_time");
        static const string trade_amount_("trade_amount");
        static const string order_local_id_("order_local_id");
        static const string order_status_("order_status");
        static const string order_submit_status_("order_submit_status");
        static const string order_type_("order_type");
        auto report_type = "order_report";
        auto order_xtp_id = to_string(order_info.order_xtp_id);
        auto order_client_id = to_string(order_info.order_client_id);
        auto order_cancel_client_id = to_string(order_info.order_cancel_client_id);
        auto order_cancel_xtp_id = to_string(order_info.order_cancel_xtp_id);
        auto ticker = order_info.ticker;
        auto market = market_type_to_string(order_info.market);
        auto price = to_string(order_info.price);
        auto quantity = to_string(order_info.quantity);
        auto price_type = price_type_to_string(order_info.price_type);
        auto side = side_type_to_string(order_info.side);
        auto position_effect = position_effect_type_to_string(order_info.position_effect);
        auto business_type = business_type_to_string(order_info.business_type);
        auto qty_traded = to_string(order_info.qty_traded);
        auto qty_left = to_string(order_info.qty_left);
        auto insert_time = to_string(order_info.insert_time);
        auto update_time = to_string(order_info.update_time);
        auto cancel_time = to_string(order_info.cancel_time);
        auto trade_amount = to_string(order_info.trade_amount);
        auto order_local_id = order_info.order_local_id;
        auto order_status = order_status_type_to_string(order_info.order_status);
        auto order_submit_status = order_submit_status_type_to_string(order_info.order_submit_status);
        auto order_type = order_type_type_to_string(order_info.order_type);
        return vector<string>{report_type_, report_type, order_xtp_id_, order_xtp_id, order_client_id_, order_client_id,
                              order_cancel_client_id_, order_cancel_client_id, order_cancel_xtp_id_,
                              order_cancel_xtp_id, ticker_, ticker, market_, market, price_, price, quantity_, quantity,
                              price_type_, price_type, side_, side, position_effect_, position_effect, business_type_,
                              business_type, qty_traded_, qty_traded, qty_left_, qty_left, insert_time_, insert_time,
                              update_time_, update_time, cancel_time_, cancel_time, trade_amount_, trade_amount,
                              order_local_id_, order_local_id, order_status_, order_status, order_submit_status_,
                              order_submit_status, order_type_, order_type};
    }

    OrderInfo::OrderInfo(XTPOrderInfo *order_info) {
        order_xtp_id = order_info->order_xtp_id;
        order_client_id = order_info->order_client_id;
        order_cancel_client_id = order_info->order_cancel_client_id;
        order_cancel_xtp_id = order_info->order_cancel_xtp_id;
        ticker = order_info->ticker;
        market = xtp_market_type_to_market_type(order_info->market);
        price = order_info->price;
        quantity = order_info->quantity;
        price_type = xtp_price_type_to_price_type(order_info->price_type);
        side = xtp_side_type_to_side_type(order_info->side);
        position_effect = xtp_position_effect_type_to_position_effect_type(order_info->position_effect);
        business_type = xtp_business_type_to_business_type(order_info->business_type);
        qty_traded = order_info->qty_traded;
        qty_left = order_info->qty_left;
        insert_time = order_info->insert_time;
        update_time = order_info->update_time;
        cancel_time = order_info->cancel_time;
        trade_amount = order_info->trade_amount;
        order_local_id = order_info->order_local_id;
        order_status = xtp_order_status_type_to_order_status_type(order_info->order_status);
        order_submit_status = xtp_order_submit_status_type_to_order_subimit_status_type(
                order_info->order_submit_status);
        order_type = txt_order_type_type_to_order_type_type(order_info->order_type);
    }

    OrderInfo::OrderInfo(const OrderInfo &other) {
        order_xtp_id = other.order_xtp_id;
        order_client_id = other.order_client_id;
        order_cancel_client_id = other.order_cancel_client_id;
        order_cancel_xtp_id = other.order_cancel_xtp_id;
        ticker = other.ticker;
        market = other.market;
        price = other.price;
        quantity = other.quantity;
        price_type = other.price_type;
        side = other.side;
        position_effect = other.position_effect;
        business_type = other.business_type;
        qty_traded = other.qty_traded;
        qty_left = other.qty_left;
        insert_time = other.insert_time;
        update_time = other.update_time;
        cancel_time = other.cancel_time;
        trade_amount = other.trade_amount;
        order_local_id = other.order_local_id;
        order_status = other.order_status;
        order_submit_status = other.order_submit_status;
        order_type = other.order_type;
    }

    json OrderInfo::to_json(const ats::OrderInfo &report) {
        json result;
        result["report_type"] = "order_report";
        result["order_xtp_id"] = report.order_xtp_id;
        result["order_client_id"] = report.order_client_id;
        result["order_cancel_client_id"] = report.order_cancel_client_id;
        result["order_cancel_xtp_id"] = report.order_cancel_xtp_id;
        result["ticker"] = report.ticker;
        result["market"] = report.market;
        result["price"] = report.price;
        result["quantity"] = report.quantity;
        result["price_type"] = price_type_to_string(report.price_type);
        result["side"] = side_type_to_string(report.side);
        result["position_effect"] = position_effect_type_to_string(report.position_effect);
        result["business_type"] = business_type_to_string(report.business_type);
        result["qty_traded"] = report.qty_traded;
        result["qty_left"] = report.qty_left;
        result["insert_time"] = report.insert_time;
        result["update_time"] = report.update_time;
        result["cancel_time"] = report.cancel_time;
        result["traded_amount"] = report.trade_amount;
        result["order_local_id"] = report.order_local_id;
        result["order_status"] = order_status_type_to_string(report.order_status);
        result["order_submit_status"] = order_submit_status_type_to_string(report.order_submit_status);
        result["order_type"] = order_type_type_to_string(report.order_type);
        return result;
    }

}
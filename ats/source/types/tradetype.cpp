//
// Created by xhshu on 8/2/18.
//

#include "tradetype.h"

namespace ats {

    using namespace std;

    string txt_trade_type_type_to_string(TXTPTradeTypeType txtp_trade_type_type) {
        switch (txtp_trade_type_type) {
            case XTP_TRDT_COMMON:
                return "XTP_TRDT_COMMON";
            case XTP_TRDT_CASH:
                return "XTP_TRDT_CASH";
            case XTP_TRDT_PRIMARY:
                return "XTP_TRDT_PRIMARY";
            default:
                return "UNKNOWN";
        }
    }

    string trade_type_type_to_string(TradeTypeType trade_type_type) {
        switch (trade_type_type) {
            case TradeTypeType::TRDT_COMMON:
                return "TRDT_COMMON";
            case TradeTypeType::TRDT_CASH:
                return "TRDT_CASH";
            case TradeTypeType::TRDT_PRIMARY:
                return "TRDT_PRIMARY";
            default:
                return "UNKNOWN";
        }
    }

    TradeTypeType string_to_trade_type_type(const string &str) {
        if (str == "TRDT_COMMON")
            return TradeTypeType::TRDT_COMMON;
        if (str == "TRDT_CASH")
            return TradeTypeType::TRDT_CASH;
        if (str == "TRDT_PRIMARY")
            return TradeTypeType::TRDT_PRIMARY;
        return TradeTypeType::UNKNOWN;
    }

    TradeTypeType txtp_trade_type_type_to_trade_type_type(TXTPTradeTypeType txtp_trade_type_type) {
        switch (txtp_trade_type_type) {
            case XTP_TRDT_COMMON:
                return TradeTypeType::TRDT_COMMON;
            case XTP_TRDT_CASH:
                return TradeTypeType::TRDT_CASH;
            case XTP_TRDT_PRIMARY:
                return TradeTypeType::TRDT_PRIMARY;
            default:
                return TradeTypeType::UNKNOWN;
        }
    }

    TXTPTradeTypeType trade_type_type_to_txtp_trade_type_type(TradeTypeType trade_type_type) {
        switch (trade_type_type) {
            case TradeTypeType::TRDT_COMMON:
                return XTP_TRDT_COMMON;
            case TradeTypeType::TRDT_CASH:
                return XTP_TRDT_CASH;
            case TradeTypeType::TRDT_PRIMARY:
                return XTP_TRDT_PRIMARY;
            default:
                return XTP_TRDT_COMMON;
        }
    }

}

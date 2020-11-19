//
// Created by xhshu on 8/2/18.
//

#ifndef ATS_TRADETYPE_H
#define ATS_TRADETYPE_H

#include <string>
#include <xtp_api_struct.h>

namespace ats {

    enum class TradeTypeType {
        TRDT_COMMON,
        TRDT_CASH,
        TRDT_PRIMARY,
        UNKNOWN
    };

    std::string txt_trade_type_type_to_string(TXTPTradeTypeType txtp_trade_type_type);

    std::string trade_type_type_to_string(TradeTypeType trade_type_type);

    TradeTypeType string_to_trade_type_type(const std::string &str);

    TradeTypeType txtp_trade_type_type_to_trade_type_type(TXTPTradeTypeType txtp_trade_type_type);

    TXTPTradeTypeType trade_type_type_to_txtp_trade_type_type(TradeTypeType trade_type_type);
};


#endif //ATS_TRADETYPE_H

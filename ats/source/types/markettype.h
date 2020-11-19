//
// Created by Rong Zhou on 2018-08-03.
//

#ifndef ATS_MARKETTYPE_H
#define ATS_MARKETTYPE_H

#include <string>
#include <xtp_api_struct.h>

namespace ats {

    enum class MarketType {
        SH,
        SZ,
        UNKNOWN
    };

    MarketType string_to_market_type(const std::string &str);

    MarketType xtp_exchange_type_to_market_type(XTP_EXCHANGE_TYPE exchange);

    MarketType xtp_market_type_to_market_type(XTP_MARKET_TYPE market);

    std::string market_type_to_string(MarketType market);

    XTP_EXCHANGE_TYPE market_type_to_xtp_exchange_type(MarketType market);

    XTP_MARKET_TYPE market_type_to_xtp_market_type(MarketType market);

}

#endif //ATS_MARKETTYPE_H

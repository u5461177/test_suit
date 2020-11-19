//
// Created by Rong Zhou on 2018-08-03.
//

#include "markettype.h"

namespace ats {

    using namespace std;

    MarketType string_to_market_type(const string &str) {
        if (str == "SH") {
            return MarketType::SH;
        }
        if (str == "SZ") {
            return MarketType::SZ;
        }
        return MarketType::UNKNOWN;
    }

    MarketType xtp_exchange_type_to_market_type(XTP_EXCHANGE_TYPE exchange) {
        switch (exchange) {
            case XTP_EXCHANGE_SH:
                return MarketType::SH;
            case XTP_EXCHANGE_SZ:
                return MarketType::SZ;
            default:
                return MarketType::UNKNOWN;
        }
    }

    MarketType xtp_market_type_to_market_type(XTP_MARKET_TYPE market) {
        switch (market) {
            case XTP_MKT_SH_A:
                return MarketType::SH;
            case XTP_MKT_SZ_A:
                return MarketType::SZ;
            default:
                return MarketType::UNKNOWN;
        }
    }

    string market_type_to_string(MarketType market) {
        switch (market) {
            case MarketType::SH:
                return "SH";
            case MarketType::SZ:
                return "SZ";
            default:
                return "UNKNOWN";
        }
    }

    XTP_EXCHANGE_TYPE market_type_to_xtp_exchange_type(MarketType market) {
        switch (market) {
            case MarketType::SH:
                return XTP_EXCHANGE_SH;
            case MarketType::SZ:
                return XTP_EXCHANGE_SZ;
            default:
                return XTP_EXCHANGE_UNKNOWN;
        }
    }

    XTP_MARKET_TYPE market_type_to_xtp_market_type(MarketType market) {
        switch (market) {
            case MarketType::SH:
                return XTP_MKT_SH_A;
            case MarketType::SZ:
                return XTP_MKT_SZ_A;
            default:
                return XTP_MKT_UNKNOWN;
        }
    }

}
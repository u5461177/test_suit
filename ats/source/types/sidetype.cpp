//
// Created by Rong Zhou on 2018-08-03.
//

#include "sidetype.h"

namespace ats {

    using namespace std;

    SideType string_to_side_type(const string &str) {
        if (str == "BUY") {
            return SideType::BUY;
        }
        if (str == "SELL") {
            return SideType::SELL;
        }
        return SideType::UNKNOWN;
    }

    SideType xtp_side_type_to_side_type(XTP_SIDE_TYPE side) {
        switch (side) {
            case XTP_SIDE_BUY:
                return SideType::BUY;
            case XTP_SIDE_SELL:
                return SideType::SELL;
            default:
                return SideType::UNKNOWN;
        }
    }

    string side_type_to_string(SideType side) {
        switch (side) {
            case SideType::BUY:
                return "BUY";
            case SideType::SELL:
                return "SELL";
            default:
                return "UNKNOWN";
        }
    }

    XTP_SIDE_TYPE side_type_to_xtp_side_type(SideType side) {
        switch (side) {
            case SideType::BUY:
                return XTP_SIDE_BUY;
            case SideType::SELL:
                return XTP_SIDE_SELL;
            default:
                return XTP_SIDE_UNKNOWN;
        }
    }

}
//
// Created by Rong Zhou on 2018-08-03.
//

#include "pricetype.h"

namespace ats {

    using namespace std;

    PriceType string_to_price_type(const string &str) {
        if (str == "LIMIT") {
            return PriceType::LIMIT;
        }
        if (str == "BEST_OR_CANCEL") {
            return PriceType::BEST_OR_CANCEL;
        }
        if (str == "BEST5_OR_LIMIT") {
            return PriceType::BEST5_OR_LIMIT;
        }
        if (str == "BEST5_OR_CANCEL") {
            return PriceType::BEST5_OR_CANCEL;
        }
        if (str == "ALL_OR_CANCEL") {
            return PriceType::ALL_OR_CANCEL;
        }
        if (str == "FORWARD_BEST") {
            return PriceType::FORWARD_BEST;
        }
        if (str == "REVERSE_BEST_LIMIT") {
            return PriceType::REVERSE_BEST_LIMIT;
        }
        if (str == "LIMIT_OR_CANCEL") {
            return PriceType::LIMIT_OR_CANCEL;
        }
        return PriceType::UNKNOWN;
    }

    PriceType xtp_price_type_to_price_type(XTP_PRICE_TYPE price_type) {
        switch (price_type) {
            case XTP_PRICE_LIMIT:
                return PriceType::LIMIT;
            case XTP_PRICE_BEST_OR_CANCEL:
                return PriceType::BEST_OR_CANCEL;
            case XTP_PRICE_BEST5_OR_LIMIT:
                return PriceType::BEST5_OR_LIMIT;
            case XTP_PRICE_BEST5_OR_CANCEL:
                return PriceType::BEST5_OR_CANCEL;
            case XTP_PRICE_ALL_OR_CANCEL:
                return PriceType::ALL_OR_CANCEL;
            case XTP_PRICE_FORWARD_BEST:
                return PriceType::FORWARD_BEST;
            case XTP_PRICE_REVERSE_BEST_LIMIT:
                return PriceType::REVERSE_BEST_LIMIT;
            case XTP_PRICE_LIMIT_OR_CANCEL:
                return PriceType::LIMIT_OR_CANCEL;
            default:
                return PriceType::UNKNOWN;
        }
    }

    string price_type_to_string(PriceType price_type) {
        switch (price_type) {
            case PriceType::LIMIT:
                return "LIMIT";
            case PriceType::BEST_OR_CANCEL:
                return "BEST_OR_CANCEL";
            case PriceType::BEST5_OR_LIMIT:
                return "BEST5_OR_LIMIT";
            case PriceType::BEST5_OR_CANCEL:
                return "BEST5_OR_CANCEL";
            case PriceType::ALL_OR_CANCEL:
                return "ALL_OR_CANCEL";
            case PriceType::FORWARD_BEST:
                return "FORWARD_BEST";
            case PriceType::REVERSE_BEST_LIMIT:
                return "REVERSE_BEST_LIMIT";
            case PriceType::LIMIT_OR_CANCEL:
                return "LIMIT_OR_CANCEL";
            default:
                return "UNKNOWN";
        }
    }

    XTP_PRICE_TYPE price_type_to_xtp_price_type(PriceType price_type) {
        switch (price_type) {
            case PriceType::LIMIT:
                return XTP_PRICE_LIMIT;
            case PriceType::BEST_OR_CANCEL:
                return XTP_PRICE_BEST_OR_CANCEL;
            case PriceType::BEST5_OR_LIMIT:
                return XTP_PRICE_BEST5_OR_LIMIT;
            case PriceType::BEST5_OR_CANCEL:
                return XTP_PRICE_BEST5_OR_CANCEL;
            case PriceType::ALL_OR_CANCEL:
                return XTP_PRICE_ALL_OR_CANCEL;
            case PriceType::FORWARD_BEST:
                return XTP_PRICE_FORWARD_BEST;
            case PriceType::REVERSE_BEST_LIMIT:
                return XTP_PRICE_REVERSE_BEST_LIMIT;
            case PriceType::LIMIT_OR_CANCEL:
                return XTP_PRICE_LIMIT_OR_CANCEL;
            default:
                return XTP_PRICE_TYPE_UNKNOWN;
        }
    }

    string price_action_to_string(PriceAction price_action) {
        switch (price_action) {
            case PriceAction::ACTIVE:
                return "ACTIVE";
            case PriceAction::PASSIVE:
                return "PASSIVE";
            default:
                return "UNKNOWN";

        }
    }


}
//
// Created by Rong Zhou on 2018-08-03.
//

#ifndef ATS_PRICETYPE_H
#define ATS_PRICETYPE_H

#include <string>
#include <xtp_api_struct.h>

namespace ats {

    enum class PriceType {
        LIMIT,
        BEST_OR_CANCEL,
        BEST5_OR_LIMIT,
        BEST5_OR_CANCEL,
        ALL_OR_CANCEL,
        FORWARD_BEST,
        REVERSE_BEST_LIMIT,
        LIMIT_OR_CANCEL,
        UNKNOWN
    };
    enum class PriceAction {
        ACTIVE,
        PASSIVE,
        UNKNOWN
    };

    PriceType string_to_price_type(const std::string &str);

    PriceType xtp_price_type_to_price_type(XTP_PRICE_TYPE price_type);

    std::string price_type_to_string(PriceType price_type);

    XTP_PRICE_TYPE price_type_to_xtp_price_type(PriceType price_type);

    std::string price_action_to_string(PriceAction price_action);


}

#endif //ATS_PRICETYPE_H

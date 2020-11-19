//
// Created by Rong Zhou on 2018-08-03.
//

#ifndef ATS_SIDETYPE_H
#define ATS_SIDETYPE_H

#include <string>
#include <xtp_api_struct.h>

namespace ats {

    enum class SideType {
        BUY,
        SELL,
        UNKNOWN
    };

    enum class BusinessSideType {
        CASH_BUY,
        CASH_SELL,
        REPO_SELL,
        UNKNOWN

    };

    SideType string_to_side_type(const std::string &str);

    SideType xtp_side_type_to_side_type(XTP_SIDE_TYPE side);

    std::string side_type_to_string(SideType side);

    XTP_SIDE_TYPE side_type_to_xtp_side_type(SideType side);

}

#endif //ATS_SIDETYPE_H

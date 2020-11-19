//
// Created by xhshu on 8/2/18.
//

#ifndef ATS_BUSINESSTYPE_H
#define ATS_BUSINESSTYPE_H


#include <string>
#include <xtp_api_struct.h>
namespace ats {
    enum class BusinessType {
        CASH,
        IPOS,
        REPO,
        ETF,
        MARGIN,
        DESIGNATION,
        ALLOTMENT,
        STRUCTURED_FUND_PURCHASE_REDEMPTION,
        STRUCTURED_FUND_SPLIT_MERGE,
        MONEY_FUND,
        OPTION,
        EXECUTE,
        FREEZE,
        UNKNOWN
    };

    std::string xtp_business_type_to_string(XTP_BUSINESS_TYPE xtp_business_type);

    std::string business_type_to_string(BusinessType business_type);

    BusinessType string_to_business_type(std::string businessType);

    XTP_BUSINESS_TYPE business_type_to_xtp_business_type(BusinessType business_type);

    BusinessType xtp_business_type_to_business_type(XTP_BUSINESS_TYPE xtp_business_type);

}


#endif //ATS_BUSINESSTYPE_H

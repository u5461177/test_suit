//
// Created by xhshu on 8/2/18.
//

#include "businesstype.h"


namespace ats {

    using namespace std;

    std::string xtp_business_type_to_string(XTP_BUSINESS_TYPE business_type) {
        switch (business_type) {
            case XTP_BUSINESS_TYPE::XTP_BUSINESS_TYPE_CASH:
                return "XTP_BUSINESS_TYPE_CASH";
            case XTP_BUSINESS_TYPE::XTP_BUSINESS_TYPE_IPOS:
                return "XTP_BUSINESS_TYPE_IPOS";
            case XTP_BUSINESS_TYPE::XTP_BUSINESS_TYPE_REPO:
                return "XTP_BUSINESS_TYPE_REPO";
            case XTP_BUSINESS_TYPE::XTP_BUSINESS_TYPE_ETF:
                return "XTP_BUSINESS_TYPE_ETF";
            case XTP_BUSINESS_TYPE::XTP_BUSINESS_TYPE_MARGIN:
                return "XTP_BUSINESS_TYPE_MARGIN";
            case XTP_BUSINESS_TYPE::XTP_BUSINESS_TYPE_DESIGNATION:
                return "XTP_BUSINESS_TYPE_DESIGNATION";
            case XTP_BUSINESS_TYPE::XTP_BUSINESS_TYPE_ALLOTMENT:
                return "XTP_BUSINESS_TYPE_ALLOTMENT";
            case XTP_BUSINESS_TYPE::XTP_BUSINESS_TYPE_STRUCTURED_FUND_PURCHASE_REDEMPTION:
                return "XTP_BUSINESS_TYPE_STRUCTURED_FUND_PURCHASE_REDEMPTION";
            case XTP_BUSINESS_TYPE::XTP_BUSINESS_TYPE_STRUCTURED_FUND_SPLIT_MERGE:
                return "XTP_BUSINESS_TYPE_STRUCTURED_FUND_SPLIT_MERGE";
            case XTP_BUSINESS_TYPE::XTP_BUSINESS_TYPE_MONEY_FUND:
                return "XTP_BUSINESS_TYPE_MONEY_FUND";
            case XTP_BUSINESS_TYPE::XTP_BUSINESS_TYPE_OPTION:
                return "XTP_BUSINESS_TYPE_OPTION";
            case XTP_BUSINESS_TYPE::XTP_BUSINESS_TYPE_EXECUTE:
                return "XTP_BUSINESS_TYPE_EXECUTE";
            case XTP_BUSINESS_TYPE::XTP_BUSINESS_TYPE_FREEZE:
                return "XTP_BUSINESS_TYPE_FREEZE";
            case XTP_BUSINESS_TYPE::XTP_BUSINESS_TYPE_UNKNOWN:
                return "XTP_BUSINESS_TYPE_UNKNOWN";
            default:
                return "UNKNOWN";
        }
    }

    std::string business_type_to_string(BusinessType business_type) {
        switch (business_type) {
            case BusinessType::CASH:
                return "CASH";
            case BusinessType::IPOS:
                return "IPOS";
            case BusinessType::REPO:
                return "REPO";
            case BusinessType::ETF:
                return "ETF";
            case BusinessType::MARGIN:
                return "MARGIN";
            case BusinessType::DESIGNATION:
                return "DESIGNATION";
            case BusinessType::ALLOTMENT:
                return "ALLOTMENT";
            case BusinessType::STRUCTURED_FUND_PURCHASE_REDEMPTION:
                return "STRUCTURED_FUND_PURCHASE_REDEMPTION";
            case BusinessType::STRUCTURED_FUND_SPLIT_MERGE:
                return "STRUCTURED_FUND_SPLIT_MERGE";
            case BusinessType::MONEY_FUND:
                return "MONEY_FUND";
            case BusinessType::OPTION:
                return "OPTION";
            case BusinessType::EXECUTE:
                return "EXECUTE";
            case BusinessType::FREEZE:
                return "FREEZE";
            default:
                return "UNKNOWN";
        }
    }

    BusinessType string_to_business_type(std::string businessType) {
        if (businessType == "CASH") {
            return BusinessType::CASH;
        }
        if (businessType == "IPOS") {
            return BusinessType::IPOS;
        }
        if (businessType == "REPO") {
            return BusinessType::REPO;
        }
        if (businessType == "ETF") {
            return BusinessType::ETF;
        }
        if (businessType == "MARGIN") {
            return BusinessType::MARGIN;
        }
        if (businessType == "DESIGNATION") {
            return BusinessType::DESIGNATION;
        }
        if (businessType == "ALLOTMENT") {
            return BusinessType::ALLOTMENT;
        }
        if (businessType == "STRUCTURED_FUND_PURCHASE_REDEMPTION") {
            return BusinessType::STRUCTURED_FUND_PURCHASE_REDEMPTION;
        }
        if (businessType == "STRUCTURED_FUND_SPLIT_MERGE") {
            return BusinessType::STRUCTURED_FUND_SPLIT_MERGE;
        }
        if (businessType == "MONEY_FUND") {
            return BusinessType::MONEY_FUND;
        }
        if (businessType == "OPTION") {
            return BusinessType::OPTION;
        }
        if (businessType == "EXECUTE") {
            return BusinessType::EXECUTE;
        }
        if (businessType == "FREEZE") {
            return BusinessType::FREEZE;
        }
        return BusinessType::UNKNOWN;
    }

    XTP_BUSINESS_TYPE business_type_to_xtp_business_type(BusinessType business_type) {
        switch (business_type) {
            case BusinessType::CASH:
                return XTP_BUSINESS_TYPE_CASH;
            case BusinessType::IPOS:
                return XTP_BUSINESS_TYPE_IPOS;
            case BusinessType::REPO:
                return XTP_BUSINESS_TYPE_REPO;
            case BusinessType::ETF:
                return XTP_BUSINESS_TYPE_ETF;
            case BusinessType::MARGIN:
                return XTP_BUSINESS_TYPE_MARGIN;
            case BusinessType::DESIGNATION:
                return XTP_BUSINESS_TYPE_DESIGNATION;
            case BusinessType::ALLOTMENT:
                return XTP_BUSINESS_TYPE_ALLOTMENT;
            case BusinessType::STRUCTURED_FUND_PURCHASE_REDEMPTION:
                return XTP_BUSINESS_TYPE_STRUCTURED_FUND_PURCHASE_REDEMPTION;
            case BusinessType::STRUCTURED_FUND_SPLIT_MERGE:
                return XTP_BUSINESS_TYPE_STRUCTURED_FUND_SPLIT_MERGE;
            case BusinessType::MONEY_FUND:
                return XTP_BUSINESS_TYPE_MONEY_FUND;
            case BusinessType::OPTION:
                return XTP_BUSINESS_TYPE_OPTION;
            case BusinessType::EXECUTE:
                return XTP_BUSINESS_TYPE_EXECUTE;
            case BusinessType::FREEZE:
                return XTP_BUSINESS_TYPE_FREEZE;
            default:
                return XTP_BUSINESS_TYPE_UNKNOWN;
        }
    }

    BusinessType xtp_business_type_to_business_type(XTP_BUSINESS_TYPE xtp_business_type) {
        switch (xtp_business_type) {
            case XTP_BUSINESS_TYPE_CASH:
                return BusinessType::CASH;
            case XTP_BUSINESS_TYPE_IPOS:
                return BusinessType::IPOS;
            case XTP_BUSINESS_TYPE_REPO:
                return BusinessType::REPO;
            case XTP_BUSINESS_TYPE_ETF:
                return BusinessType::ETF;
            case XTP_BUSINESS_TYPE_MARGIN:
                return BusinessType::MARGIN;
            case XTP_BUSINESS_TYPE_DESIGNATION:
                return BusinessType::DESIGNATION;
            case XTP_BUSINESS_TYPE_ALLOTMENT:
                return BusinessType::ALLOTMENT;
            case XTP_BUSINESS_TYPE_STRUCTURED_FUND_PURCHASE_REDEMPTION:
                return BusinessType::STRUCTURED_FUND_PURCHASE_REDEMPTION;
            case XTP_BUSINESS_TYPE_STRUCTURED_FUND_SPLIT_MERGE:
                return BusinessType::STRUCTURED_FUND_SPLIT_MERGE;
            case XTP_BUSINESS_TYPE_MONEY_FUND:
                return BusinessType::MONEY_FUND;
            case XTP_BUSINESS_TYPE_OPTION:
                return BusinessType::OPTION;
            case XTP_BUSINESS_TYPE_EXECUTE:
                return BusinessType::EXECUTE;
            case XTP_BUSINESS_TYPE_FREEZE:
                return BusinessType::FREEZE;
            default:
                return BusinessType::UNKNOWN;
        }
    }

}
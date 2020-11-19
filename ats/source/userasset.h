//
// Created by Rong Zhou on 2018-08-03.
//

#ifndef ATS_USERASSET_H
#define ATS_USERASSET_H

#include <vector>
#include <string>
#include <xtp_api_struct.h>

namespace ats {

    struct UserAsset {
        static std::vector<std::string> to_record(const UserAsset &asset);

        UserAsset(XTPQueryAssetRsp *asset);

        UserAsset(const UserAsset &other);

        double total_asset;
        double buying_power;
        double security_asset;
        double fund_buy_amount;
        double fund_buy_fee;
        double fund_sell_amount;
        double fund_sell_fee;
        double withholding_amount;
        XTP_ACCOUNT_TYPE account_type;
        double frozen_margin;
        double frozen_exec_cash;
        double frozen_exec_fee;
        double pay_later;
        double preadva_pay;
        double orig_balance;
        double balance;
        double deposit_withdraw;
        double trade_netting;
        double captial_asset;
        double force_freeze_amount;
        double preferred_amount;
    };

}

#endif //ATS_USERASSET_H

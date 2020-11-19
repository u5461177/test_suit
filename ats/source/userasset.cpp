//
// Created by Rong Zhou on 2018-08-03.
//

#include "userasset.h"

namespace ats {

    using namespace std;

    vector<string> UserAsset::to_record(const UserAsset &asset) {
        static const string total_asset_("total_asset");
        static const string buying_power_("buying_power");
        auto total_asset = to_string(asset.total_asset);
        auto buying_power = to_string(asset.buying_power);
        return {total_asset_, total_asset, buying_power_, buying_power};
    }

    UserAsset::UserAsset(XTPQueryAssetRsp *asset) {
        total_asset = asset->total_asset;
        buying_power = asset->buying_power;
        security_asset = asset->security_asset;
        fund_buy_amount = asset->fund_buy_amount;
        fund_buy_fee = asset->fund_buy_fee;
        fund_sell_amount = asset->fund_sell_amount;
        fund_sell_fee = asset->fund_buy_fee;
        withholding_amount = asset->withholding_amount;
        account_type = asset->account_type;
        frozen_margin = asset->frozen_margin;
        frozen_exec_cash = asset->frozen_exec_cash;
        frozen_exec_fee = asset->frozen_exec_fee;
        pay_later = asset->pay_later;
        preadva_pay = asset->preadva_pay;
        orig_balance = asset->orig_banlance;
        balance = asset->banlance;
        deposit_withdraw = asset->deposit_withdraw;
        trade_netting = asset->trade_netting;
        captial_asset = asset->captial_asset;
        force_freeze_amount = asset->force_freeze_amount;
        preferred_amount = asset->preferred_amount;
    }

    UserAsset::UserAsset(const UserAsset &other) {
        total_asset = other.total_asset;
        buying_power = other.buying_power;
        security_asset = other.security_asset;
        fund_buy_amount = other.fund_buy_amount;
        fund_buy_fee = other.fund_buy_fee;
        fund_sell_amount = other.fund_sell_amount;
        fund_sell_fee = other.fund_sell_fee;
        withholding_amount = other.withholding_amount;
        account_type = other.account_type;
        frozen_margin = other.frozen_margin;
        frozen_exec_cash = other.frozen_exec_cash;
        frozen_exec_fee = other.frozen_exec_fee;
        pay_later = other.pay_later;
        preadva_pay = other.preadva_pay;
        orig_balance = other.orig_balance;
        balance = other.balance;
        deposit_withdraw = other.deposit_withdraw;
        trade_netting = other.trade_netting;
        captial_asset = other.captial_asset;
        force_freeze_amount = other.force_freeze_amount;
        preferred_amount = other.preferred_amount;
    }

}
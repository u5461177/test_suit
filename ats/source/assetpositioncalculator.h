//
// Created by xtgu on 4/22/19.
//

#ifndef MUSE_ASSETPOSITIONCALCULATOR_H
#define MUSE_ASSETPOSITIONCALCULATOR_H


#include <memory>
#include <mutex>
#include "orderinfo.h"
#include "singleton.h"
#include "types/errorcode.h"
#include "util/gloghelper.h"
#include "order.h"


namespace ats {
    class AssetPositionCalculator {
    public:
        explicit AssetPositionCalculator(ats::SideType &side_type);

        virtual ~AssetPositionCalculator() = default;


    protected:
        ats::SideType _sideType;


    public:
        virtual bool update_asset(const std::shared_ptr<ats::OrderInfo> &orderInfo, double &buying_power_add,
                                  double &withdraw_amount) = 0;

        virtual bool update_position(int64_t qty_left, SideType side, int64_t &result) = 0;

        virtual double amount_fee(double traded_amount) = 0;

        virtual double amount_with_fee(double traded_amount) = 0;

        virtual int64_t update_quantity_with_amount(Quantity &quantity, double &ticker_trade_price,
                                                    double left_amount, int64_t placed_qty, double price) = 0;

        virtual double get_correct_price(double price) = 0;

        virtual int get_minimal_qty() const = 0;

        virtual double get_min_trade_cost() const = 0;

        virtual double get_tax_rate() const = 0;

        virtual uint64_t quantity_integer(uint64_t quantity) =0;

    };


}


#endif //MUSE_ASSETPOSITIONCALCULATOR_H

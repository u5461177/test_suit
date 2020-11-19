//
// Created by xtgu on 7/24/19.
//

#ifndef MUSE_CASHCAPITALCALCULATOR_H
#define MUSE_CASHCAPITALCALCULATOR_H

#include "assetpositioncalculator.h"


namespace ats {
    class CashCapitalCalculator : public AssetPositionCalculator {

    private:
        static constexpr double _min_trade_cost = 5.0;
        static constexpr double _tax_rate = 0.003;
        static constexpr double _stamp_tax_rate = 0.001;
        static constexpr int _minimal_qty = 100;

    public:
        explicit CashCapitalCalculator(ats::SideType &sideType);

        ~CashCapitalCalculator() override = default;

    public:


        bool update_asset(const std::shared_ptr<ats::OrderInfo> &orderInfo, double &buying_power_add,
                          double &withdraw_amount) override;

        bool update_position(int64_t qty_left, SideType side, int64_t &result) override;

        double amount_fee(double traded_amount) override;

        double amount_with_fee(double traded_amount) override;


        int64_t update_quantity_with_amount(Quantity &quantity, double &ticker_trade_price,
                                            double left_amount, int64_t placed_qty, double price) override;

        double get_correct_price(double price) override;

        int get_minimal_qty() const override;

        double get_min_trade_cost() const override;

        double get_tax_rate() const override;

        uint64_t quantity_integer(uint64_t quantity) override;
    };

    inline int CashCapitalCalculator::get_minimal_qty() const {
        return _minimal_qty;
    }

    inline double CashCapitalCalculator::get_correct_price(double price) {
        return price;
    }

    inline double CashCapitalCalculator::get_min_trade_cost() const {
        return _min_trade_cost;
    }

    inline double CashCapitalCalculator::get_tax_rate() const {
        return _tax_rate;
    }

    inline uint64_t CashCapitalCalculator::quantity_integer(uint64_t quantity) {

        return (quantity/_minimal_qty)*_minimal_qty;
    }

}


#endif //MUSE_CASHCAPITALCALCULATOR_H

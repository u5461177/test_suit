//
// Created by xtgu on 7/24/19.
//

#ifndef MUSE_REPOCAPITALCALCULATOR_H
#define MUSE_REPOCAPITALCALCULATOR_H

#include "assetpositioncalculator.h"


namespace ats {

    class RepoCapitalCalculator : public AssetPositionCalculator {
    private:
        static constexpr double _min_trade_cost = 3.0;
        static constexpr double _tax_rate = 0.002;
        static constexpr int _repo_price = 100;
    private:
        int _minimal_qty;

    public:
        explicit RepoCapitalCalculator(ats::SideType &sideType, ats::MarketType &marketType);

        ~RepoCapitalCalculator() override = default;

    public:
        bool update_asset(const std::shared_ptr<ats::OrderInfo> &orderInfo, double &buying_power_add,
                          double &withdraw_amount) override;

        bool update_position(int64_t qty_left, SideType side, int64_t &result) override;

        double amount_fee(double traded_amount) override;

        double amount_with_fee(double traded_amount) override;

        int64_t update_quantity_with_amount(Quantity &quantity, double &ticker_trade_price, double left_amount,
                                            int64_t placed_qty, double price) override;

        double get_correct_price(double price) override;

        int get_minimal_qty() const override;

        double get_min_trade_cost() const override;

        double get_tax_rate() const override;

        uint64_t quantity_integer(uint64_t quantity) override;


    };

    inline int RepoCapitalCalculator::get_minimal_qty() const {
        return _minimal_qty;
    }

    inline double RepoCapitalCalculator::get_min_trade_cost() const {
        return _min_trade_cost;
    }

    inline double RepoCapitalCalculator::get_tax_rate() const {
        return _tax_rate;
    }

    inline uint64_t RepoCapitalCalculator::quantity_integer(uint64_t quantity) {

        return (quantity/_minimal_qty)*_minimal_qty;
    }

}


#endif //MUSE_REPOCAPITALCALCULATOR_H

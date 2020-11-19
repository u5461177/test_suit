//
// Created by Rong Zhou on 2018-08-03.
//

#ifndef ATS_VWAPTASK_H
#define ATS_VWAPTASK_H

#include "tracktask.h"
#include "istask.h"

namespace ats {

    class User;

    class VWAPTask : public TrackTask {
    public:
        VWAPTask(std::shared_ptr<TaskInputData> task_input);

        ~VWAPTask() override = default;

    public:
        Type get_type() const override;

        void start(const nlohmann::json &params) override;

    private:
        std::string database_verify();

        void predict_market_quote();

        void limit_participate_rate();

        void estimate_risk_value();

        double func_diff_hx(double param_p);

        void volume_calibration();

        double hamiltonain_system(double lambda);

        void ac_frame();

        void save();

    protected:
        int64_t calc_accumulative_quantity() override;

        int64_t get_total_quantity() override;

    private:
        double _risk_aversion;
        double _eta;
        double _psi;
        double _phi;
        double _rho;
        double _k;
        bool _stop_strategy;
        size_t _time_len;
        double _total_market_vol;
        int64_t _start_volume;
        int64_t _original_start_volume;
        double _history_price;
        std::vector<double> _volume_curve;
        std::vector<double> _remained_vol_list;
        std::vector<double> _predict_mktvol;
        std::vector<double> _expect_price_volatility;
        std::vector<double> _lambda_list;
        std::vector<double> _shooting_lambda_list;
        std::vector<double> _last_volume_list;
    };

    inline Type VWAPTask::get_type() const {
        return Type::VWAP;
    }

}

#endif //ATS_VWAPTASK_H

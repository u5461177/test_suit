//
// Created by Rong Zhou on 2018-08-03.
//

#ifndef ATS_ISTASK_H
#define ATS_ISTASK_H

#include "tracktask.h"

namespace ats {

    using namespace std::chrono;
    using json = nlohmann::json;

    class ISTask : public TrackTask {
    public:
        ISTask(std::shared_ptr<TaskInputData> task_input);

        ~ISTask() override = default;

    public:
        Type get_type() const override;

        void start(const nlohmann::json &params) override;

    protected:
        int64_t calc_accumulative_quantity() override;

        int64_t get_total_quantity() override;

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

    private:
        double _risk_aversion;
        double _eta;
        double _psi;
        double _phi;
        double _rho;
        bool _stop_strategy;
        size_t _time_len;
        double _total_market_vol;
        double _history_price;
        int64_t _start_volume;
        int64_t _original_start_volume;
        size_t _risk_adjust;
        std::vector<double> _volume_curve;
        std::vector<double> _remained_vol_list;
        std::vector<double> _predict_mktvol;
        std::vector<double> _expect_price_volatility;
        std::vector<double> _lambda_list;
        std::vector<double> _shooting_lambda_list;
        std::vector<double> _last_volume_list;
    };

    inline Type ISTask::get_type() const {
        return Type::IS;
    }

}

#endif //ATS_ISTASK_H

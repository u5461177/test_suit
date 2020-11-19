//
// Created by xhshu on 11/1/18.
//

#ifndef ATS_TRADITIONALVWAP_H
#define ATS_TRADITIONALVWAP_H

#include "tracktask.h"

namespace ats {

    class TraditionalVWAP : public TrackTask {
    public:
        TraditionalVWAP(std::shared_ptr<TaskInputData> task_input);

        ~TraditionalVWAP() override = default;

    public:
        Type get_type() const override;

        void start(const nlohmann::json &params) override;

    private:
        void predict_market_quote();

        std::string database_verify();

    protected:
        int64_t calc_accumulative_quantity() override;

    private:
        std::chrono::system_clock::duration _total_time;
        double _total_market_vol;
        double _history_price;
        size_t _time_len;
        std::vector<double> _predict_mktvol;
        std::vector<double> _cum_per_mkt_vol;
    };

    inline Type TraditionalVWAP::get_type() const {
        return Type::VWAP;
    }

}


#endif //ATS_TRADITIONALVWAP_H

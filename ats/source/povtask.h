//
// Created by rongzhou on 2018-08-03.
//

#ifndef ATS_POVTASK_H
#define ATS_POVTASK_H

#include "tracktask.h"

namespace ats {

    class User;

    class POVTask : public TrackTask {
    public:
        POVTask(std::shared_ptr<TaskInputData> task_input);

        ~POVTask() override = default;

    public:
        void start(const nlohmann::json &params) override;

    protected:
        Type get_type() const override;

        void update_market_data(std::unique_ptr<MarketData> market_data) override;

        int64_t calc_accumulative_quantity() override;

        int64_t get_total_quantity() override;

    private:
        double _participation_rate;
        int64_t _start_quantity;
        int64_t _current_quantity;
        double _start_amount;
        double _current_amount;
    };

    inline Type POVTask::get_type() const {
        return Type::POV;
    }

}

#endif //ATS_POVTASK_H

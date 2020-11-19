//
// Created by rongzhou on 2018-08-03.
//

#ifndef ATS_RANDOMTASK_H
#define ATS_RANDOMTASK_H

#include "task.h"
#include "marketdata.h"

namespace ats {

    class RandomTask : public Task {
    public:
        RandomTask(std::shared_ptr<TaskInputData> task_input);

        ~RandomTask() final = default;

    public:
        void start(const nlohmann::json &params) override;

    protected:
        Type get_type() const override;

        bool execution() override;

        bool _break_time_check() override;

    private:
        int64_t calc_current_trading_quantity();

        std::chrono::system_clock::time_point calc_next_trading_time();

    private:
        int64_t _num_of_tradings;
        int64_t _num_of_tradings_left;
        std::chrono::system_clock::time_point _next_trading_time;
    };

    inline Type RandomTask::get_type() const {
        return Type::RANDOM;
    }

}

#endif //ATS_RANDOMTASK_H

//
// Created by Rong Zhou on 2018-08-03.
//

#ifndef ATS_TWAPTASK_H
#define ATS_TWAPTASK_H

#include "tracktask.h"
#include "taskinputdata.h"

namespace ats {

    class User;

    class TWAPTask : public TrackTask {

    public:
        TWAPTask(std::shared_ptr<TaskInputData> task_input);


        ~TWAPTask() override = default;

    public:
        Type get_type() const override;

        void start(const nlohmann::json &params) override;

    protected:
        int64_t calc_accumulative_quantity() override;

        int64_t get_total_quantity() override;
    };

    inline Type TWAPTask::get_type() const {
        return Type::TWAP;
    }

}

#endif //ATS_TWAPTASK_H

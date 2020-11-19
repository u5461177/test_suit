//
// Created by Rong Zhou on 2018-08-03.
//

#ifndef ATS_TRACKTASK_H
#define ATS_TRACKTASK_H

#include "task.h"
#include "marketdata.h"

namespace ats {

    class TrackTask : public Task {
        static constexpr double _max_volatility = 0.05;
        static constexpr double _max_lost_volatility = 0.1;

    public:
        TrackTask(std::shared_ptr<TaskInputData> task_input);

        ~TrackTask() override = default;

    protected:
        bool execution() override;

        virtual int64_t calc_accumulative_quantity() = 0;

        virtual int64_t get_total_quantity() = 0;

        virtual void recalculate_order_quantity_price(std::pair<int64_t, double> &qty_price, int64_t left_qty, long time_left);

    protected:
        bool _first_flag;
    };

    inline void TrackTask::recalculate_order_quantity_price(std::pair<int64_t, double> &qty_price, int64_t left_qty,
            long time_left) {
    }

}

#endif //ATS_TRACKTASK_H

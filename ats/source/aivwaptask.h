//
// Created by xtgu on 6/6/19.
//

#ifndef MUSE_AIVWAP_H
#define MUSE_AIVWAP_H

#include "vwaptask.h"
#include "state.h"
#include "quoteserver.h"
#include "util/gloghelper.h"
#include "types/errorcode.h"
#include "configuration.h"
#include "torch/script.h"
#include <bits/ios_base.h>
#include <fstream>
#include <cmath>

namespace ats {

    struct AIOrder {
        double price;
        int64_t quantity;
    };

    class AIVWAPTask : public VWAPTask {
    public:
        AIVWAPTask(std::shared_ptr<TaskInputData> task_input);

        Type get_type() const override;

        ~AIVWAPTask() override = default;

    protected:

        int8_t _guess(std::shared_ptr<State> state);

        std::shared_ptr<AIOrder> _guess_order(std::shared_ptr<State> state);

        void recalculate_order_quantity_price(std::pair<int64_t, double> &qty_price, int64_t left_qty,
                                              long time_left) override;

    private:
        std::shared_ptr<torch::jit::script::Module> _module = nullptr;

    };

    inline Type AIVWAPTask::get_type() const {
        return Type::AIVWAP;
    }

}


#endif //MUSE_AIVWAP_H

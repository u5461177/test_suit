//
// Created by xhshu on 8/27/18.
//

#ifndef ATS_ICEBERGTASK_H
#define ATS_ICEBERGTASK_H

#include "task.h"
#include "marketdata.h"

namespace ats {

    class IcebergTask : public Task {
    public:
        IcebergTask(std::shared_ptr<TaskInputData> task_input);

        ~IcebergTask() final = default;

    public:
        void start(const nlohmann::json &params) override;

    protected:
        Type get_type() const override;

        bool execution() override;

    private:
        int64_t calc_current_trading_quantity();

    private:
        uint32_t _num_of_tradings_left;
        int64_t _ordered_quantity;
        double _limit_price;
        int64_t _visible_volume;
        double _visible_amount;
        bool _amount_trade_flag;
    };

    inline Type IcebergTask::get_type() const {
        return Type::ICEBERG;
    }
}
#endif //ATS_ICEBERGTASK_H
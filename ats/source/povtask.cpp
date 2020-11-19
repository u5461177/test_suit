//
// Created by rongzhou on 2018-08-03.
//

#include "povtask.h"
#include "util/timehelper.h"
#include "glog/logging.h"

namespace ats {

    using namespace std;
    using namespace std::chrono;
    using time_point = std::chrono::system_clock::time_point;
    using json = nlohmann::json;

    POVTask::POVTask(shared_ptr<TaskInputData> task_input)
            : TrackTask(task_input) {
        _participation_rate = 0.0;
        _start_quantity = 0;
        _current_quantity = 0;
        _start_amount = 0.0;
        _current_amount = 0.0;
    }

    void POVTask::start(const json &params) {
        _input_start_time = string_to_time_point(params["start_time"].get<string>());
        _input_end_time = string_to_time_point(params["end_time"].get<string>());
        _participation_rate = params["participation_rate"].get<double>();
        if (_amount_flag) {
            _amount = params["amount"].get<double>();
            _minimal_trade_amount = params["minimal_trade_amount"].get<double>();
            _left_amount = _amount;
        } else {
            _minimal_trade_quantity = params["minimal_trade_quantity"].get<int64_t>();
        }
        if (params.find("maximal_qty") != params.end()) {
            _maximal_qty = params["maximal_qty"].get<int64_t>();
            LOG(INFO) << "maximal quantity: " << _maximal_qty << " | task id: " << _task_id;
        }
        parameter_parse();
        _total_time = calc_time_span(_end_time) - calc_time_span(_start_time);
        TrackTask::start(params);
    }

    void POVTask::update_market_data(unique_ptr<MarketData> market_data) {
        if (_start_quantity == 0 or _start_amount == 0.0) {
            _start_quantity = market_data->qty;
            _start_amount = market_data->turnover;
        }
        _current_quantity = market_data->qty;
        _current_amount = market_data->turnover;
        TrackTask::update_market_data(move(market_data));
    }

    int64_t POVTask::calc_accumulative_quantity() {
        auto accum_qty = _participation_rate * (_current_quantity - _start_quantity);
        return min((int64_t) accum_qty, _quantity.first);
    }

    int64_t POVTask::get_total_quantity() {
        return _quantity.first;
    }
}
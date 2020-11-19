//
// Created by Rong Zhou on 2018-08-03.
//

#include "twaptask.h"
#include "util/timehelper.h"
#include <glog/logging.h>

namespace ats {

    using namespace std;
    using namespace std::chrono;
    using json = nlohmann::json;

    TWAPTask::TWAPTask(std::shared_ptr<ats::TaskInputData> task_input) : TrackTask(task_input){

    }

    void TWAPTask::start(const json &params) {
        _input_start_time = string_to_time_point(params["start_time"].get<string>());
        _input_end_time = string_to_time_point(params["end_time"].get<string>());
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

    int64_t TWAPTask::calc_accumulative_quantity() {
        LOG(INFO) << "start to calc accumulative quantity | task id: " << _task_id;
        auto elapsed_time = calc_time_span(system_clock::now()) - calc_time_span(_start_time);
        LOG(INFO) << "start_time: " <<time_point_to_string(_start_time)<< " | " << _task_id;
        LOG(INFO)<< "elapsed_time : " << elapsed_time.count() << "|" << "total_time: " <<_total_time.count() << _task_id;
        return (int64_t) (_quantity.first * (1.0 * min(elapsed_time, _total_time) / _total_time));
    }

    int64_t TWAPTask::get_total_quantity() {
        return _quantity.first;
    }

}
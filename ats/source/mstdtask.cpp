//
// Created by Ruiqi Zhang on 2019-05-28
//

#include "mstdtask.h"
#include "util/timehelper.h"
#include <glog/logging.h>
#include "marketdata.h"
#include "quoteserver.h"
#include "reposerver.h"
#include "user.h"

namespace ats {

    using namespace std;
    using namespace std::chrono;
    using json = nlohmann::json;

    MSTDTask::MSTDTask(shared_ptr<TaskInputData> task_input)
            : TrackTask(task_input) {
    }

    void MSTDTask::start(const json &params) {
        _input_start_time = string_to_time_point(params["start_time"].get<string>());
        _input_end_time = string_to_time_point(params["end_time"].get<string>());
        if (_amount_flag) {
            _amount = params["amount"].get<double>();
            _minimal_trade_amount = params["minimal_trade_amount"].get<double>();
            _left_amount = _amount;
        } else {
            _minimal_trade_quantity = params["minimal_trade_quantity"].get<int64_t>();
            _minimal_trade_amount = _minimal_trade_quantity*100;
        }
        if (params.find("maximal_qty") != params.end()) {
            _maximal_qty = params["maximal_qty"].get<int64_t>();
            LOG(INFO) << "maximal quantity: " << _maximal_qty << " | task id: " << _task_id;
        }
        parameter_parse();
        _total_time = calc_time_span(_end_time) - calc_time_span(_start_time);
        TrackTask::start(params);
    }

    bool MSTDTask::execution() {
            int64_t left_qty = _quantity.first - _traded_order_quantity - _pending_qty;
            LOG(INFO) << "Left_qty " << left_qty <<"| _minimal_trade_amount:" << _minimal_trade_amount<< " | task id: "<<_task_id;
            if (left_qty < _minimal_trade_amount / 100) {
                LOG(WARNING) << "Left quantity less than minimal trade quantity" << " | task id: " << _task_id;
                if (_pending_qty > 0 ) {
                    this_thread::sleep_for(3s);
                    return false;
                }
                cancel_all_orders();
                task_status_change(_task_id, Status::FINISHED);
                return false;
            }

            if (_end_time - system_clock::now() <= 20s) {
                LOG(INFO) << "Place order in last 20s ." << " | task id:" << _task_id;
                place_order(_other_party_best_price, left_qty);
                this_thread::sleep_for(10s);
                cancel_all_orders();
                return false;
            }

            double duration = std::chrono::duration_cast<std::chrono::minutes>(_end_time - system_clock::now()).count();
            double mean = RepoServer::instance().get_repo_mean(_ticker.first);
            double std = RepoServer::instance().get_repo_std(_ticker.first);
            if (!(_other_party_best_price > (mean + 3 * std) || duration < 5.)) {
                this_thread::sleep_for(10s);
                return false;
            }


            if (_placed_qty < _quantity.first)
            {
                if(!place_order(_other_party_best_price, left_qty)) {
                    this_thread::sleep_for(1s);
                    return false;
                }
            }
            update_order_status();
            cancel_all_orders();
            this_thread::sleep_for(1s);
        return true;

    }

    int64_t MSTDTask::calc_accumulative_quantity() {
        LOG(INFO) << "start to calc accumulative quantity | task id: " << _task_id;
        auto elapsed_time = calc_time_span(system_clock::now()) - calc_time_span(_start_time);
        return (int64_t) (_quantity.first * (1.0 * min(elapsed_time, _total_time) / _total_time));
    }
}

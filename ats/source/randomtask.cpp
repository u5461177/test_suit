//
// Created by rongzhou on 2018-08-03.
//

#include "randomtask.h"
#include <random>
#include "user.h"
#include "quoteserver.h"
#include "util/timehelper.h"
#include "glog/logging.h"

namespace ats {

    using namespace std;
    using namespace std::chrono;
    using time_point = std::chrono::system_clock::time_point;
    using json = nlohmann::json;

    RandomTask::RandomTask(shared_ptr<TaskInputData> task_input)
            : Task(task_input) {
        _num_of_tradings = 0;
        _num_of_tradings_left = 0;
    }

    void RandomTask::start(const json &params) {
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
        _num_of_tradings = params["num_of_tradings"].get<uint32_t>();
        parameter_parse();
        _total_time = calc_time_span(_end_time) - calc_time_span(_start_time);
        LOG(INFO) << "num_of_tradings is: " << _num_of_tradings << " | task id: " << _task_id;
        _next_trading_time = _start_time;
        _num_of_tradings_left = _num_of_tradings;
        Task::start(params);
    }

    bool RandomTask::execution() {
            auto trading_qty = calc_current_trading_quantity();
            if (trading_qty > 0) {
                if (_this_party_best_price > 0) {
                    if (!place_order(_this_party_best_price, trading_qty)) {
                        LOG(ERROR) << "order place error | task_id: " << _task_id;
                        this_thread::sleep_for(1s);
                        return false;
                    }
                }
                int64_t withdraw_qty = _traded_order_quantity + _hot_order_quantity + trading_qty - _quantity.first;
                LOG(WARNING) << "The length of xtp_id is: " << _xtp_order_id_orders.size() << " | task id: "
                             << _task_id;
                LOG(WARNING) << "traded vol qty is: " << _traded_order_quantity << " | task id: " << _task_id;
                while (withdraw_qty > 0) {
                    auto worst_order = find_worst_order();
                    if (worst_order.first == 0) {
                        LOG(ERROR) << "cannot find worst order | task id: " << _task_id;
                        on_task_stop();
                        break;
                    }
                    cancel_order(worst_order.first);
                    withdraw_qty -= worst_order.second;
                }
                if (--_num_of_tradings_left > 0) {
                    _next_trading_time = calc_next_trading_time();
                } else {
                    _next_trading_time = _end_time;
                    return false;
                }
            }
            this_thread::sleep_for(1s);
            return true;

    }

    int64_t RandomTask::calc_current_trading_quantity() {
        LOG(INFO) << "start to calc accumulative quantity | task id: " << _task_id;
        int64_t left_qty = 0;
        int minimal_qty = _asset_position_calculator->get_minimal_qty();
        double min_trade_cost = _asset_position_calculator->get_min_trade_cost();
        double tax_rate = _asset_position_calculator->get_tax_rate();
        if (_amount_flag) {
            if (_quantity.second == SideType::BUY or
                (_business_type == BusinessType::REPO and _quantity.second == SideType::SELL)) {
                left_qty = (int64_t) (floor(((_left_amount - _asset_position_calculator->amount_fee(_left_amount))
                                             / _ticker_trade_price) / minimal_qty) * minimal_qty);
            } else if (_quantity.second == SideType::SELL) {
                left_qty = (int64_t) (ceil(((_left_amount + max(min_trade_cost, _left_amount * tax_rate))
                                            / _ticker_trade_price) / minimal_qty) * minimal_qty);
            }
        } else {
            left_qty = _quantity.first - _traded_order_quantity - _hot_order_quantity;
        }
        _update_placed_qty();
        if (_placed_qty < _quantity.first) {
            if (_num_of_tradings_left > 1 && left_qty > 0) {
                random_device device;
                mt19937 generator(device());
                int64_t mean_qty = left_qty / _num_of_tradings_left;
                int64_t variance_qty = mean_qty / 10;
                binomial_distribution<int64_t> distribution(mean_qty, variance_qty);
                auto trading_qty = distribution(generator);
                trading_qty = (trading_qty < _minimal_trade_quantity) ? _minimal_trade_quantity : trading_qty;
                if (trading_qty + _minimal_trade_quantity > left_qty) {
                    trading_qty = left_qty;
                }
                if (_placed_qty + trading_qty / minimal_qty * minimal_qty > _quantity.first) {
                    trading_qty = _quantity.first - _placed_qty;
                }
                return trading_qty / minimal_qty * minimal_qty;
            }
            if (_placed_qty + left_qty > _quantity.first) {
                left_qty = _quantity.first - _placed_qty;
            }
            return left_qty;
        }
        return 0;
    }

    time_point RandomTask::calc_next_trading_time() {
        random_device device;
        mt19937 generator(device());
        auto duration = (calc_time_span(_end_time) - calc_time_span(_next_trading_time)) / (_num_of_tradings_left + 1);
        auto mean_time = _next_trading_time + duration;
        auto variance_time = duration / 10;
        binomial_distribution<int64_t> distribution(0, duration_cast<seconds>(variance_time).count());
        auto variance_seconds = distribution(generator);
        auto next_trading_time = mean_time + seconds(variance_seconds);
        if (next_trading_time >= _break_start_time and next_trading_time < _break_end_time) {
            next_trading_time = _break_end_time;
        }
        LOG(WARNING) << "place time is: " << time_point_to_string(next_trading_time)
                     << " | task id: " << _task_id;
        return next_trading_time;
    }


    bool RandomTask::_break_time_check() {
        _num_of_tradings_left = _num_of_tradings - (int64_t) _order_num;
        if (_num_of_tradings_left > 0 and _next_trading_time == _end_time) {
            _next_trading_time = system_clock::now();
        } else if (_num_of_tradings_left <= 0) {
            _next_trading_time = _end_time;
        }
        if (system_clock::now() < _next_trading_time or
            (system_clock::now() >= _break_start_time and system_clock::now() < _break_end_time)) {
            this_thread::sleep_for(1s);
            return true;
        }
        return false;

    }
}

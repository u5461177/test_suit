//
// Created by xhshu on 8/27/18.
//

#include "icebergtask.h"
#include <cmath>
#include "user.h"
#include "quoteserver.h"
#include "glog/logging.h"   // glog 头文件

namespace ats {

    using namespace std;
    using namespace std::chrono;
    using time_point = std::chrono::system_clock::time_point;
    using json = nlohmann::json;

    IcebergTask::IcebergTask(shared_ptr<TaskInputData> task_input)
            : Task(task_input) {
        _ordered_quantity = 0;
        _traded_order_quantity = 0;
        _num_of_tradings_left = 0;
        _visible_amount = 0.0;
    }

    void IcebergTask::start(const json &params) {
        _input_start_time = string_to_time_point(params["start_time"].get<string>());
        _input_end_time = string_to_time_point(params["end_time"].get<string>());
        if (_amount_flag) {
            _amount = params["amount"].get<double>();
            _visible_amount = params["visible_amount"].get<double>();
            _left_amount = _amount;
        }
        if (params.find("maximal_qty") != params.end()) {
            _market_qty = params["maximal_qty"].get<int64_t>();
            LOG(INFO) << "maximal quantity: " << _maximal_qty << " | task id: " << _task_id;
        }
        auto visible_volume = params["visible_volume"].get<int64_t>();
        _visible_volume = visible_volume / _asset_position_calculator->get_minimal_qty() *
                _asset_position_calculator->get_minimal_qty();
        LOG(INFO) << "visible volume: " << _visible_volume;
        _num_of_tradings_left = (uint32_t) (ceil((_quantity.first + 0.0) / _visible_volume));
        LOG(INFO) << "left num: " << _num_of_tradings_left;
        parameter_parse();
        _total_time = calc_time_span(_end_time) - calc_time_span(_start_time);
        Task::start(params);
    }

    bool IcebergTask::execution() {
            if (_placed_qty < _quantity.first) {
                if (_traded_order_quantity == _ordered_quantity || _order_num == _trade_num) {
                    if (auto trading_qty = calc_current_trading_quantity()) {
                        if (_num_of_tradings_left >= 0) {
                            if (_this_party_best_price > 0) {
                                if (trading_qty + _placed_qty > _quantity.first) {
                                    trading_qty = _placed_qty - _quantity.first;
                                }
                                if (!place_order(_this_party_best_price, trading_qty)) {
                                    this_thread::sleep_for(1s);
                                    return false;
                                }
                            }
                            _ordered_quantity += trading_qty;
                        }
                    }
                    this_thread::sleep_for(1s);
                    return false;
                }
            }
            this_thread::sleep_for(1s);
            return true;

    }

    int64_t IcebergTask::calc_current_trading_quantity() {
        LOG(INFO) << "start to calc accumulative quantity | task id: " << _task_id;
        int minimal_qty = _asset_position_calculator->get_minimal_qty();
        if (_amount_flag) {
            if (_ticker_trade_price > 0.0) {
                if (_businessSideType == BusinessSideType::CASH_BUY or _businessSideType == BusinessSideType::REPO_SELL) {
                    _visible_volume = (int64_t) (
                            floor((_visible_amount - _asset_position_calculator->amount_fee(_visible_amount))
                                  / _ticker_trade_price / minimal_qty) * minimal_qty);
                    _num_of_tradings_left = (uint32_t) (ceil(_left_amount / _visible_amount));

                    if (_num_of_tradings_left-- > 1) {
                        return _visible_volume;
                    }
                    return (int64_t) (floor((_left_amount -_asset_position_calculator->amount_fee(_left_amount))
                                            / _ticker_trade_price / minimal_qty) * minimal_qty);
                } else if (_businessSideType == BusinessSideType::CASH_SELL) {
                    _visible_volume = (int64_t) (
                            ceil((_visible_amount + _asset_position_calculator->amount_fee(_visible_amount))
                                 / _ticker_trade_price / minimal_qty) * minimal_qty);
                    _num_of_tradings_left = (uint32_t) (ceil(_left_amount / _visible_amount));
                    if (_num_of_tradings_left-- > 1) {
                        return _visible_volume;
                    }
                    return (int64_t) (ceil((_left_amount - _asset_position_calculator->amount_fee(_left_amount))
                                           / _ticker_trade_price / minimal_qty) * minimal_qty);
                }
            }
            return 0;
        } else {
            auto left_qty = _quantity.first - _traded_order_quantity;
            _num_of_tradings_left = (uint32_t) (ceil((left_qty + 0.0) / _visible_volume));
            LOG(INFO) << "left qty: " << left_qty << " | num of trading left: "
                      << _num_of_tradings_left << " | task id: " << _task_id;
            if (_num_of_tradings_left-- > 1) {
                return (int64_t) (_visible_volume / minimal_qty * minimal_qty);
            }
            return left_qty;
        }
    }
}

//
// Created by Rong Zhou on 2018-08-03.
//

#include "tracktask.h"
#include "user.h"
#include "quoteserver.h"
#include "glog/logging.h"
#include "taskinputdata.h"

#include <iostream>
#include <fstream>
namespace ats {

    using namespace std;
    using namespace std::chrono;
    using time_point = std::chrono::system_clock::time_point;

    TrackTask::TrackTask(shared_ptr<TaskInputData> task_input)
            : Task(task_input) {
    }

    bool TrackTask::execution() {
            int64_t total_qty = 0;
            int64_t accum_qty = 0;
            int64_t adjust_qty = 0;
            int64_t current_qty = 0;
            int64_t passive_qty = 0;
            int64_t aggressive_qty = 0;
            int64_t left_qty = 0;
            double target_traded_progress;
            double target_traded_quantity;

            total_qty = get_total_quantity();
            accum_qty = calc_accumulative_quantity();

            if (get_type() == Type::IS) {
                adjust_qty = (int64_t) (_minimal_trade_quantity * (total_qty - accum_qty) / total_qty);
                LOG(INFO) << "IS adjust qty: " << adjust_qty << " | task id: " << _task_id;
            } else {
                LOG(INFO) << (_refer_cancel_freq.count() + 0.0) / _total_time.count() * total_qty << " | "
                          << (_max_volatility * (total_qty - accum_qty));
                adjust_qty = min((int64_t) (((_refer_cancel_freq.count() + 0.0) / _total_time.count()) * total_qty),
                                 (int64_t) (_max_volatility * (total_qty - accum_qty)));
            }

            current_qty = accum_qty + adjust_qty - _traded_order_quantity - _hot_order_quantity;
            passive_qty = min(current_qty, (int64_t) max((_max_volatility * (total_qty - accum_qty) * 2 -
                                                          _hot_order_quantity), 0.0));
            LOG(INFO) << "current_qty | " << current_qty << "| max_volatility | " << _max_volatility << "| total_qty | "
            << total_qty << "| accum_qty | " << accum_qty << "| hot_order_quantity | " << _hot_order_quantity << "| passive_qty | " << passive_qty;
            aggressive_qty = current_qty - passive_qty;
            target_traded_progress = 1 - _max_lost_volatility * (max(0.0, (1 - 1.0 * accum_qty / total_qty)));
            target_traded_quantity = accum_qty * target_traded_progress;

            LOG(INFO) << "traded vol: " << _traded_order_quantity << " | target quantity volume: "
                      << target_traded_quantity << " | task id: " << _task_id;
            if (target_traded_quantity > _traded_order_quantity and
                (calc_time_span(system_clock::now()) - calc_time_span(_last_time)) > _refer_cancel_freq) {
                LOG(INFO) << "start to place market order for meeting target trade qty | task id: " << _task_id;
                cancel_cold_orders();
                aggressive_qty = (int64_t) (target_traded_quantity) - _traded_order_quantity;
                _last_time = system_clock::now();
            }
            int64_t minimal_qty = _asset_position_calculator->get_minimal_qty();
            aggressive_qty = aggressive_qty / minimal_qty * minimal_qty;
            if (aggressive_qty < _minimal_trade_quantity) {
                aggressive_qty = 0;
            }
            passive_qty = current_qty - aggressive_qty;
            passive_qty = passive_qty / minimal_qty * minimal_qty;
            if (passive_qty < _minimal_trade_quantity) {
                passive_qty = 0;
                if (_first_order_flag) {
                    passive_qty = _minimal_trade_quantity / minimal_qty * minimal_qty;
                    if (passive_qty > total_qty) {
                        passive_qty = total_qty;
                    }
                    if (passive_qty <= minimal_qty) {
                        passive_qty = minimal_qty;
                    }
                    _first_order_flag = false;
                }
            }
            left_qty = total_qty - _traded_order_quantity - _hot_order_quantity - aggressive_qty - passive_qty;
            if (left_qty <= _minimal_trade_quantity) {
                passive_qty += left_qty;
            }

            auto time_spread = calc_time_span(_end_time) - calc_time_span(system_clock::now());
            long time_left = time_spread.count();
            // TODO: place all market order at the end of the task (10s)
            if (!cancel_last_order_flag) {
                aggressive_qty += passive_qty;
                passive_qty = 0;
            }

            if (_placed_qty <= total_qty) {
                if (aggressive_qty > 0) {
                    pair<int64_t, double> qty_price = make_pair(aggressive_qty, _other_party_best_price);
                    aggressive_qty = qty_price.first;
                    double best_price = qty_price.second;
                    if (_placed_qty + aggressive_qty > total_qty) {
                        aggressive_qty = total_qty - _placed_qty;
                    }
                    if (_other_party_best_price > 0) {
                        LOG(INFO) << "In place aggressive | " << aggressive_qty << "task id: " <<_task_id << endl;
                        if (!place_order(best_price, aggressive_qty)) {
                            this_thread::sleep_for(1s);
                            return false;
                        }
                    }
                }
                if (passive_qty > 0) {
                    pair<int64_t, double> qty_price = make_pair(passive_qty, _this_party_best_price);
                    recalculate_order_quantity_price(qty_price, left_qty, time_left);
                    passive_qty = qty_price.first;
                    double best_price = qty_price.second;
                    if (_placed_qty + passive_qty > total_qty) {
                        passive_qty = total_qty - _placed_qty;
                    }

                    if (_this_party_best_price > 0) {
                        LOG(INFO) << "In place passive | " << passive_qty<< "task id: " <<_task_id << endl;
                        if (!place_order(best_price, passive_qty)) {
                            this_thread::sleep_for(1s);
                            return false;
                        }
                    }
                }
            }

            int64_t executed_qty = _traded_order_quantity + _hot_order_quantity + _cold_order_quantity;
            int64_t withdraw_qty = executed_qty + aggressive_qty + passive_qty - total_qty;
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
            LOG(INFO) << "total qty: " << total_qty << " | task id: " << _task_id;
            LOG(INFO) << "traded quantity: " << _traded_order_quantity << " | task id: " << _task_id;
            LOG(INFO) << "hot quantity: " << _hot_order_quantity << " | task id: " << _task_id;
            LOG(INFO) << "cold quantity: " << _cold_order_quantity << " | task id: " << _task_id;
            LOG(INFO) << "target quantity: " << accum_qty << " | task id: " << _task_id;
            LOG(INFO) << "adjust quantity: " << adjust_qty << " | task id: " << _task_id;
            LOG(INFO) << "aggressive qty: " << aggressive_qty << " | task id: " << _task_id;
            LOG(INFO) << "passive qty: " << passive_qty << " | task id: " << _task_id;
            LOG(INFO) << "left_qty: " << left_qty << " | task id: " << _task_id;
            LOG(INFO) << "pending qty: " << _pending_qty << " | task id: " << _task_id;
            // TODO: last time cancel all orders  (10s)
            if (_end_time - system_clock::now() <= 10s and cancel_last_order_flag) {
                cancel_all_orders();
                cancel_last_order_flag = false;
            }
            this_thread::sleep_for(1s);
        return true;
        }

}
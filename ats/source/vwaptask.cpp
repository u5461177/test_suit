//
// Created by Rong Zhou on 2018-08-03.
//

#include <fstream>
#include <algorithm>
#include "vwaptask.h"
#include "volumepredictor.h"
#include "util/timehelper.h"
#include "configuration.h"
#include "glog/logging.h"

namespace ats {

    using namespace std;
    using json = nlohmann::json;

    VWAPTask::VWAPTask(std::shared_ptr<TaskInputData> task_input)
            : TrackTask(task_input) {
        _stop_strategy = false;
        _risk_aversion = 1e-6;
        _k = 0.0;
        _eta = 0.1;
        _psi = 0;
        _phi = 0.5;
        _rho = 0.15;
        _original_start_volume = 0;
    }

    void VWAPTask::start(const json &params) {
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
        _start_time = _input_start_time;
        _end_time = _input_end_time;
        // TODO: For open test
        parameter_parse();

        _total_time = calc_time_span(_end_time) - calc_time_span(_start_time);
        _current_date = get_current_date();
        _start_volume = _quantity.first;
        _original_start_volume = _quantity.first;
        LOG(INFO) << "qty is: " << _quantity.first;

        if (_total_time.count() <= 0 || _status == Status::STOPPED) {
            _stop_strategy = true;
            on_task_stop();
        } else if (database_verify() == "OK") {
            _time_len = (size_t) (_total_time.count() / 1000000000 / 60);
            LOG(INFO) << "time length is: " << _time_len;
            predict_market_quote();
            if (_total_market_vol > 0) {
                _k = 0.05 / _total_market_vol;
                limit_participate_rate();
                estimate_risk_value();
                ac_frame();
                if (_stop_strategy) {
                    LOG(ERROR) << "algorithm error: stock: " << _ticker.first << " | qty: " << _quantity.first;
                    _error_code = ErrorCategory(ErrorCode::ALGORITHM_PARAMETER_ERROR);
                    {
                        std::lock_guard<std::mutex> guard(_remark_mutex);
                        _remark = _error_code.message();
                    }
                    on_task_stop();
                } else {
                    TrackTask::start(params);
                }
            } else {
                _stop_strategy = true;
                _remark = "Estimated volume is insufficient";
                LOG(ERROR) << _remark << " | task_id: " << _task_id;
                on_task_stop();
            }
        } else {
            _stop_strategy = true;
            _remark = database_verify();
            LOG(ERROR) << _remark << " | task_id: " << _task_id;
            on_task_stop();
        }
        //on_task_report(_input_start_time, _input_end_time);
        LOG(INFO) << "remark is: " << _remark;
    }

    int64_t VWAPTask::calc_accumulative_quantity() {
        LOG(INFO) << "start to calc accumulative quantity | task id: " << _task_id;
        auto elapsed_time = calc_time_span(system_clock::now()) - calc_time_span(_start_time);
        if (!_stop_strategy) {
            int64_t adjust_quantity = _quantity.first - _original_start_volume;
            LOG(INFO) << "VWAP adjust start quantity: " << adjust_quantity << " | task id: " << _task_id;
            if (_volume_curve.size() >= 2) {
                uint64_t size = _volume_curve.size() - 1;
                uint64_t index = min((uint64_t) ceil(size * (1.0 * min(elapsed_time, _total_time) / _total_time)),
                                     size);
                LOG(INFO) << "volume size: " << _volume_curve.size() << " index: " << index << " | size: " << size <<
                          " | first: " << _volume_curve[0] << " | last: " << _volume_curve[size] <<
                          "| task id: " << _task_id;
                return max(min((int64_t) (_volume_curve[index]), _start_volume) + adjust_quantity, (int64_t) 0);
            }
            return max(min((int64_t) (_volume_curve[0]), _start_volume) + adjust_quantity, (int64_t) 0);
        }
        return 0;
    }

    int64_t VWAPTask::get_total_quantity() {
        return min(_start_volume, _quantity.first);
    }

    string VWAPTask::database_verify() {
        double total_market_vol = 0.0;
        auto result = VolumePredictor::get_instance()->check_exist(_current_date, _ticker.first, total_market_vol);
        if (!result) {
            LOG(ERROR) << "stock has no quote: " << _ticker.first;
            return "Estimated volume is not available";
        }
        if (total_market_vol < 10000000) {
            LOG(ERROR) << "no enough market volume | stock: " << _ticker.first << " total market vol is: "
                       << total_market_vol;
            return "Estimated volume is insufficient";
        }
        return "OK";
    }

    void VWAPTask::predict_market_quote() {
        vector<uint64_t> time_pieces;
        _predict_mktvol.clear();
        _expect_price_volatility.clear();
        std::vector<double> predict_mktvol;
        std::vector<double> expect_price_volatility;
        auto start_time = _start_time;
        auto end_time = _end_time;
        // TODO: For open test
        //  let it work when in non-trade_time
        if (!Configuration::instance().get_trade_time_limit()) {
            if (start_time > string_to_time_point("14:45:00") || start_time < string_to_time_point("09:30:00") ||
                end_time > string_to_time_point("14:50:00") || end_time < string_to_time_point("09:30:00")) {
                start_time = string_to_time_point("09:30:00");
                end_time = start_time + (calc_time_span(_end_time) - calc_time_span(_start_time));
            }
            if (end_time > _break_start_time and end_time < _break_end_time) {
                end_time = _break_end_time + (end_time - _break_start_time);
            }
            if (end_time > string_to_time_point("14:50:00")) {
                end_time = string_to_time_point("14:50:00");
            }
            _total_time = calc_time_span(end_time) - calc_time_span(start_time);
            _time_len = (size_t) (_total_time.count() / 1000000000 / 60);
            LOG(INFO) << "time length is: " << _time_len;
        }

        make_unique<VolumePredictor>()->get_predict_data(
                1,
                _current_date,
                _ticker.first,
                predict_mktvol,
                expect_price_volatility,
                time_pieces,
                _history_price
        );
        auto start_count = calc_time_span(start_time) - calc_time_span(string_to_time_point("09:30:00"));
        auto end_count = calc_time_span(end_time) - calc_time_span(string_to_time_point("09:30:00"));
        auto start_index = (size_t) (start_count.count() / 1000000000 / 60);
        auto end_index = (size_t) (end_count.count() / 1000000000 / 60);
        LOG(INFO) << "start index: " << start_index << " | end index: " << end_index;
        for (size_t i = 0; i < time_pieces.size(); i++) {
            if (i >= start_index and i < end_index) {
                _predict_mktvol.push_back(predict_mktvol[i]);
                _expect_price_volatility.push_back(expect_price_volatility[i]);
            }
        }
        predict_mktvol.clear();
        expect_price_volatility.clear();
        LOG(INFO) << "the size of time list is: " << _expect_price_volatility.size();
        _total_market_vol = accumulate(_predict_mktvol.begin(), _predict_mktvol.end(), 0.0);
        LOG(INFO) << "total market volume is: " << _total_market_vol;
    }

    void VWAPTask::limit_participate_rate() {
        if (_start_volume / _total_market_vol > 0.1) {
            LOG(WARNING) << "start volume over than 10% of predicted market vol in this period";
            _start_volume = (int64_t) (_total_market_vol * 0.1 / 100) * 100;
            _minimal_trade_quantity = min(_minimal_trade_quantity, _start_volume);
            LOG(WARNING) << "adjusted start volume is: " << _start_volume;
            _error_code = ErrorCategory(ErrorCode::QUANTITY_TOO_LARGE);
            {
                std::lock_guard<std::mutex> guard(_remark_mutex);
                _remark = _error_code.message();
            }
        }
        _rho = _start_volume / _total_market_vol * 1.5;
    }

    void VWAPTask::estimate_risk_value() {
//        _risk_aversion = -1.0 * 1e+6 / (_total_market_vol * _history_price * _history_price);
        _risk_aversion = -1.0 * 1e+4 / (_start_volume * _history_price * _history_price);
        LOG(INFO) << "risk aversion value: " << _risk_aversion;
    }

    double VWAPTask::func_diff_hx(double param_p) {
        auto index = 1 / _phi;
        auto item_1 = _rho;
        auto item_3 = _rho / 3;
        auto max_val = max(abs(param_p) - _psi, 0.0);
        auto item_2 = pow((max_val / ((_phi + 1) * _eta)), index);
        double result = max(item_3, min(item_1, item_2)) * (_shooting_lambda_list[0] < 0 ? -1 : 1);
        return result;
    }

    void VWAPTask::volume_calibration() {
        _volume_curve.clear();
        size_t i = 0;
        for (const auto vol:_remained_vol_list) {
            if (vol * _start_volume < 0) {
                _stop_strategy = true;
            }
            if ((_start_volume - vol) * _start_volume >= 0 && (i < _remained_vol_list.size() - 1)) {
                _volume_curve.push_back(_start_volume - vol);
            } else if ((_start_volume - vol) * _start_volume >= 0 && (i == _remained_vol_list.size() - 1)) {
                _volume_curve.push_back(_start_volume);
            } else {
                LOG(ERROR) << "order vol pieces have different signal: " << _start_volume << " | " << vol
                           << " VWAP: stock: " << _ticker.first << " | qty: " << _quantity.first << " | start time: "
                           << time_point_to_string(_start_time) << " | end time: " << time_point_to_string(_end_time);
                _stop_strategy = true;
                break;
            }
            i++;
        }
        // TODO:
        assert(_remained_vol_list.size() - 1 >= _time_len);
        assert(_volume_curve.size() - 1 >= _time_len);
    }

    double VWAPTask::hamiltonain_system(double lambda) {
        double update_volume = _start_volume;
        _remained_vol_list.clear();
        _remained_vol_list.push_back(_start_volume);
        vector<double> p_list{lambda};
        _lambda_list.clear();
        _lambda_list.push_back(lambda);
        for (size_t i = 0; i < _predict_mktvol.size(); i++) {
            auto update_lambda = lambda;
            auto current_mktval = accumulate(_predict_mktvol.begin(), _predict_mktvol.begin() + i + 1, 0.0);
            auto hx = func_diff_hx(update_lambda);
            update_volume += _predict_mktvol[i] * hx;
            _remained_vol_list.push_back(update_volume);
            auto risk_term = _risk_aversion * _expect_price_volatility[i] *
                             (update_volume - _start_volume * (1 - (current_mktval / _total_market_vol)));
//            risk_term = max(risk_term, 0.0);
            auto impact_term = _k * _start_volume * _predict_mktvol[i] / _total_market_vol;
            lambda = update_lambda + max((risk_term + impact_term), 0.0);
            p_list.push_back(lambda);
            _lambda_list.push_back(lambda);
        }
        return update_volume;
    }

    void VWAPTask::ac_frame() {
        struct timeval t1, t2;
        gettimeofday(&t1, nullptr);
        auto last_volume = (double) _start_volume;
        auto init_lambda = (_eta * (1 + _phi) * pow(_rho, _phi) + _psi) * (_start_volume > 0 ? -1 : 1);
        LOG(INFO) << "init lambda value: " << init_lambda;
        auto low = 0.0;
        auto high = init_lambda * 2;
        if (init_lambda < 0) {
            high = 0.0;
            low = init_lambda * 2;
        }
        bool converge_flag = false;
        bool first_flag = true;
        auto adjust_times = 0;
        auto last_temp_volume = last_volume;
        _shooting_lambda_list.push_back(init_lambda);
        while ((last_volume > min(100.0, _start_volume * 0.01)) || !converge_flag) {
            last_volume = hamiltonain_system(init_lambda);
            _last_volume_list.push_back(last_volume);
            LOG(INFO) << "last remained vol is: " << last_volume;
            if ((last_volume * _start_volume > 0) && (last_volume <= min(100.0, _start_volume * 0.01))) {
                converge_flag = true;
            } else {
                if (_lambda_list.back() == 0 and abs(last_temp_volume - last_volume) < 0.1) {
                    if (adjust_times++ >= 10) {
                        _stop_strategy = true;
                        _error_code = ErrorCategory(ErrorCode::ALGORITHM_PARAMETER_ERROR);
                        {
                            std::lock_guard<std::mutex> guard(_remark_mutex);
                            _remark = _error_code.message();
                        }
                        LOG(ERROR) << "algorithm parameter error: " << _start_volume << " | " << "stock: "
                                   << _ticker.first
                                   << " | qty: " << _quantity.first << " | start time: "
                                   << time_point_to_string(_start_time) << " | end time: "
                                   << time_point_to_string(_end_time);
                        break;
                    }
                    low = init_lambda;
                    LOG(WARNING) << "cannot arrive target, increase init lambda value. updated lambda: " << init_lambda;
                } else if (last_volume > 0 and first_flag) {
                    // determine down-limit boundary first
                    low *= 2;
                    high = init_lambda;
                    init_lambda = (low + high) / 2;
                } else {
                    first_flag = false;
                    // shooting method
                    if (last_volume > 0) {
                        high = init_lambda;
                    } else if (last_volume < 0) {
                        low = init_lambda;
                    }
                    init_lambda = (low + high) / 2;
                }
                last_temp_volume = last_volume;
                LOG(INFO) << "updated lambda value is: " << init_lambda;
                _shooting_lambda_list.push_back(init_lambda);
                _remained_vol_list.clear();
            }
            // for time out
            gettimeofday(&t2, nullptr);
            if (t2.tv_sec - t1.tv_sec > 5) {
                _stop_strategy = true;
                _error_code = ErrorCategory(ErrorCode::ALGORITHM_EXECUTE_ERROR);
                {
                    std::lock_guard<std::mutex> guard(_remark_mutex);
                    _remark = _error_code.message();
                }
                LOG(ERROR) << "algorithm error: " << _start_volume << " | " << "stock: " << _ticker.first
                           << " | qty: " << _quantity.first << " | start time: "
                           << time_point_to_string(_start_time) << " | end time: " << time_point_to_string(_end_time);
                break;
            }
        }
        volume_calibration();
    }

    void VWAPTask::save() {
        std::ofstream f_out;
        auto filename = "test_result/VWAP/" + to_string(_quantity.first) + "/" + _ticker.first + "_" +
                        to_string(_quantity.first) + "_" +
                        _current_date + "_" + time_point_to_string(_start_time) + "_" +
                        time_point_to_string(_end_time);
        f_out.open(filename + "_remained_vol_list.txt");
        for (auto i : _remained_vol_list)
            f_out << std::setprecision(10) << i << std::endl;
        f_out.close();

        std::ofstream lambda_out;
        auto lambda_filename = "test_result/VWAP/" + to_string(_quantity.first) + "/" + _ticker.first + "_" +
                               to_string(_quantity.first) + "_" +
                               _current_date + "_" + time_point_to_string(_start_time) + "_" +
                               time_point_to_string(_end_time);
        lambda_out.open(lambda_filename + "_lambda_list.txt");
        for (auto i : _lambda_list)
            lambda_out << std::setprecision(10) << i << std::endl;
        lambda_out.close();

        std::ofstream shoot_lambda_out;
        auto shoot_lambda_filename = "test_result/VWAP/" + to_string(_quantity.first) + "/" + _ticker.first + "_" +
                                     to_string(_quantity.first) + "_" +
                                     _current_date + "_" + time_point_to_string(_start_time) + "_" +
                                     time_point_to_string(_end_time);
        shoot_lambda_out.open(shoot_lambda_filename + "_shooting_lambda.txt");
        for (auto i : _shooting_lambda_list)
            shoot_lambda_out << std::setprecision(10) << i << std::endl;
        shoot_lambda_out.close();

        std::ofstream shoot_volume_out;
        auto shoot_volume_filename = "test_result/VWAP/" + to_string(_quantity.first) + "/" + _ticker.first + "_" +
                                     to_string(_quantity.first) + "_" +
                                     _current_date + "_" + time_point_to_string(_start_time) + "_" +
                                     time_point_to_string(_end_time);
        shoot_volume_out.open(shoot_volume_filename + "_shooting_volume.txt");
        for (auto i : _last_volume_list)
            shoot_volume_out << std::setprecision(10) << i << std::endl;
        shoot_volume_out.close();

        std::ofstream predict_vol_out;
        auto predict_vol_filename = "test_result/VWAP/" + to_string(_quantity.first) + "/" + _ticker.first + "_" +
                                    to_string(_quantity.first) + "_" +
                                    _current_date + "_" + time_point_to_string(_start_time) + "_" +
                                    time_point_to_string(_end_time);
        predict_vol_out.open(predict_vol_filename + "_predict_vol.txt");
        for (auto i : _predict_mktvol)
            predict_vol_out << std::setprecision(10) << i << std::endl;
        predict_vol_out.close();

        std::ofstream curve_order_volume;
        curve_order_volume.open(predict_vol_filename + "_curve_order_volume.txt");
        for (auto i : _volume_curve)
            curve_order_volume << std::setprecision(10) << i << std::endl;
        curve_order_volume.close();
    }
}
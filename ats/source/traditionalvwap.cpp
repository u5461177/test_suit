//
// Created by xhshu on 11/1/18.
//

#include "traditionalvwap.h"
#include "volumepredictor.h"
#include "util/timehelper.h"
#include "glog/logging.h"

namespace ats {

    using namespace std;
    using namespace std::chrono;
    using json = nlohmann::json;

    TraditionalVWAP::TraditionalVWAP(shared_ptr<TaskInputData> task_input)
            : TrackTask(task_input) {
        _cum_per_mkt_vol.clear();
    }

    void TraditionalVWAP::start(const json &params) {
        _input_start_time = string_to_time_point(params["start_time"]);
        _input_end_time = string_to_time_point(params["end_time"]);

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
        _time_len = (size_t) _total_time.count() / 1000000000;
        _current_date = get_current_date();
        LOG(INFO) << "time length is: " << _time_len;
        if (_time_len <= 0 || system_clock::now() > _end_time || _status == Status::STOPPED) {
            _status = Status::STOPPED;
            _error_code = ErrorCategory(ErrorCode::INVALID_TIME_PARAMETER);
            {
                std::lock_guard<std::mutex> guard(_remark_mutex);
                _remark = _error_code.message();
            }
            on_task_report(_input_start_time, _input_end_time);
            LOG(INFO) << "remark is: " << _remark;
        } else if (database_verify() == "normal") {
            predict_market_quote();
            TrackTask::start(params);
        } else {
            _status = Status::STOPPED;
            _remark = database_verify();
            on_task_report(_input_start_time, _input_end_time);
            LOG(INFO) << "remark is: " << _remark;
        }
    }

    int64_t TraditionalVWAP::calc_accumulative_quantity() {
        auto elapsed_time = calc_time_span(system_clock::now()) - calc_time_span(_start_time);
        auto index = (uint64_t) (_time_len * (1.0 * min(elapsed_time, _total_time) / _total_time));
        return (int64_t) (_cum_per_mkt_vol[index] * _quantity.first);
    }

    string TraditionalVWAP::database_verify() {
        double total_market_vol = 0.0;
        auto result = VolumePredictor::get_instance()->check_exist(_current_date, _ticker.first, total_market_vol);
        if (!result) {
            LOG(ERROR) << "stock has no quote: " << _ticker.first;
            return "no market quote";
        }
        if (total_market_vol < 10000000) {
            LOG(ERROR) << "stock is: " << _ticker.first << ", total market vol is: " << total_market_vol;
            return "no enough market volume";
        }
        return "normal";
    }

    void TraditionalVWAP::predict_market_quote() {
        vector<uint64_t> time_pieces;
        _predict_mktvol.clear();
        std::vector<double> predict_mktvol;
        auto start_time = _start_time;
        auto end_time = _end_time;
        // for open test
        if (start_time > string_to_time_point("14:45:00") || start_time < string_to_time_point("09:30:00")) {
            start_time = string_to_time_point("09:40:00");
            end_time = start_time + (calc_time_span(_end_time) - calc_time_span(_start_time));
        }
        if (end_time > string_to_time_point("14:50:00")) {
            end_time = string_to_time_point("14:50:00");
        }
        //
        make_unique<VolumePredictor>()->get_predict_volume(
                1,
                _current_date,
                _ticker.first,
                predict_mktvol,
                time_pieces,
                _history_price
        );
        auto start_count = calc_time_span(start_time) - calc_time_span(string_to_time_point("09:30:00"));
        auto end_count = calc_time_span(end_time) - calc_time_span(string_to_time_point("09:30:00"));
        auto start_index = (size_t) start_count.count() / 1000000000;
        auto end_index = (size_t) end_count.count() / 1000000000;
        for (size_t i = 0; i < time_pieces.size(); i++) {
            if (i >= start_index and i < end_index) {
                _predict_mktvol.push_back(predict_mktvol[i]);
            }
        }
        predict_mktvol.clear();
        _total_market_vol = accumulate(_predict_mktvol.begin(), _predict_mktvol.end(), 0.0);
        LOG(INFO) << "total market volume is: " << _total_market_vol;
        auto cum_per_mkt_vol = 0.0;
        for (auto &w_vol : _predict_mktvol) {
            cum_per_mkt_vol += w_vol / (_total_market_vol + 0.0);
            _cum_per_mkt_vol.push_back(cum_per_mkt_vol);
        }
        _cum_per_mkt_vol.push_back(1);
    }
}
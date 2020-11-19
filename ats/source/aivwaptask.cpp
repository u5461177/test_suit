//
// Created by xtgu on 6/6/19.
//

#include "aivwaptask.h"

namespace ats {

    using namespace std;
    using namespace std::chrono;
    using time_point = std::chrono::system_clock::time_point;
    using json = nlohmann::json;

    AIVWAPTask::AIVWAPTask(std::shared_ptr<TaskInputData> task_input)
            : VWAPTask(task_input) {
        ifstream is(Configuration::instance().get_aivwap_model(), ifstream::binary);
        try {
            _module = torch::jit::load(is);
        } catch (const exception &e) {
            LOG(ERROR) << "AIVWAP model can not be loaded in project!!" << endl;
        }
    }

    void AIVWAPTask::recalculate_order_quantity_price(pair<int64_t, double> &qty_price, const int64_t left_qty,
                                                      const long time_left) {
        LOG(INFO) << "In AIVWAPTASK" << endl;
        shared_ptr<State> state = make_shared<State>(_ticker,
                                                     QuoteServer::instance().query_market_data_queue(_ticker),
                                                     left_qty, time_left, qty_price);
        auto guess_order = this->_guess_order(state);
        int minimal_qty = _asset_position_calculator->get_minimal_qty();
        qty_price.first = guess_order->quantity / minimal_qty * minimal_qty;
        qty_price.second = guess_order->price;
    }

    int8_t AIVWAPTask::_guess(shared_ptr<State> state) {
        if (_module == nullptr) {
            LOG(ERROR) << "load pytorch model fail | task id: " << _task_id;
            return 0;
        }

        std::vector<torch::jit::IValue> inputs;
        auto tensor_input = torch::zeros({1, 503});

        for (int i = 0; i < 25; i++) {
            if (std::shared_ptr<MarketData> market_data = state->_orderbook->at((unsigned long) i)) {
                double *bid_prices = market_data->bid;
                int64_t *bid_qtys = market_data->bid_qty;
                for (int j = 0; j < 5; j = j + 2) {
                    double bid_price = bid_prices[j];
                    int64_t bid_qty = bid_qtys[j];
                    tensor_input[0][i * 20 + j] = log(bid_price + 1);
                    tensor_input[0][i * 20 + j + 1] = log(bid_qty + 1);
                }

                double *ask_prices = market_data->ask;
                int64_t *ask_qtys = market_data->ask_qty;
                for (int j = 0; j < 5; j = j + 2) {
                    double ask_price = ask_prices[j];
                    int64_t ask_qty = ask_qtys[j];
                    tensor_input[0][i * 20 + 10 + j] = log(ask_price + 1);
                    tensor_input[0][i * 20 + 10 + j + 1] = log(ask_qty + 1);
                }
            }
        }
        tensor_input[0][500] = log(state->_time_left + 1);
        tensor_input[0][501] = log(state->_quantity_left + 1);
        tensor_input[0][502] = log(state->_qty_price.first + 1);

        LOG(INFO) << "Pytorch input:" << tensor_input << " | task id: " << _task_id;
        inputs.emplace_back(tensor_input);
        try {
            auto output = torch::argmax(((this->_module)->forward(inputs)).toTensor());
            LOG(INFO) << "Pytorch output:" << output << " | task id: " << _task_id;
            return output.item<int8_t>();
        } catch (const exception &e) {
            LOG(ERROR) << e.what() << " | task id: " << _task_id;
            int8_t output = 5;
            return output;
        };
    }

    shared_ptr<AIOrder> AIVWAPTask::_guess_order(shared_ptr<State> state) {
        auto order = make_shared<AIOrder>();
        auto current_price = state->_qty_price.second;
        auto current_quantity = state->_qty_price.first;
        order->price = current_price;
        order->quantity = current_quantity;
        int action = _guess(move(state));
        if (action < 0 or action > 7) {
            LOG(ERROR) << "invalid action" << " | task id: " << _task_id;
            return order;
        }

        switch (action) {
            case 0:
                order->price = current_price - 0.05;
                break;
            case 1:
                order->price = current_price - 0.04;
                break;
            case 2:
                order->price = current_price - 0.03;
                break;
            case 3:
                order->price = current_price - 0.02;
                break;
            case 4:
                order->price = current_price - 0.01;;
                break;
            case 5:
                order->price = current_price;
                break;
            case 6:
                order->price = current_price + 0.01;
                break;
            case 7:
                order->price = current_price + 0.02;;
                break;
            default:
                order->price = current_price;
        }
        return order;
    }
}

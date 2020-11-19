//
// Created by Rong Zhou on 2018-08-03.
//

#include "task.h"
#include "user.h"
#include "redisserver.h"
#include "quoteserver.h"
#include "configuration.h"
#include "glog/logging.h"
#include "cashcapitalcalculator.h"
#include "repocapitalcalculator.h"
#include "types/orderstatustype.h"


namespace ats {

    using namespace std;
    using namespace std::chrono;
    using json = nlohmann::json;

    Type Task::string_to_type(const string &str) {
        if (str == "TWAP") {
            return Type::TWAP;
        }
        if (str == "VWAP") {
            return Type::VWAP;
        }
        if (str == "POV") {
            return Type::POV;
        }
        if (str == "IS") {
            return Type::IS;
        }
        if (str == "RANDOM") {
            return Type::RANDOM;
        }
        if (str == "ICEBERG") {
            return Type::ICEBERG;
        }
        if (str == "AIVWAP") {
            return Type::AIVWAP;
        }
        if (str == "MSTD") {
            return Type::MSTD;
        }
        return Type::UNKNOWN;
    }

    string Task::type_to_string(Type task_type) {
        switch (task_type) {
            case Type::TWAP:
                return "TWAP";
            case Type::VWAP:
                return "VWAP";
            case Type::POV:
                return "POV";
            case Type::IS:
                return "IS";
            case Type::RANDOM:
                return "RANDOM";
            case Type::ICEBERG:
                return "ICEBERG";
            case Type::AIVWAP:
                return "AIVWAP";
            case Type::MSTD:
                return "MSTD";
            default:
                return "UNKNOWN";
        }
    }

    string Task::status_to_string(Status task_status) {
        switch (task_status) {
            case Status::READY:
                return "READY";
            case Status::IN_PROGRESS:
                return "IN_PROGRESS";
            case Status::PAUSED:
                return "PAUSED";
            case Status::STOPPED:
                return "STOPPED";
            case Status::FINISHED:
                return "FINISHED";
            case Status::CAPITAL_INSUFFICIENT_ERROR:
                return "CAPITAL_INSUFFICIENT_ERROR";
            case Status::SECURITY_INSUFFICIENT_ERROR:
                return "SECURITY_INSUFFICIENT_ERROR";
            default:
                return "UNKNOWN";
        }
    }

    Task::Task(std::shared_ptr<TaskInputData> task_input) {
        _owner = task_input->_owner;
        _task_id = task_input->_task_id;
        _business_type = task_input->_business_type;
        _quantity = task_input->_quantity;
        _task_user_id = task_input->_task_user_id;
        _ticker = task_input->_ticker;
        _amount_flag = task_input->_amount_flag;
        _user_mac = task_input->_user_mac;
        time_t now = time(nullptr);
        tm t = *localtime(&now);
        // TODO: For open test
        if (Configuration::instance().get_trade_time_limit()) {
            t.tm_hour = 9;
            t.tm_min = 30;
            t.tm_sec = 0;
            _open_time = system_clock::from_time_t(mktime(&t));
            t.tm_hour = 11;
            _break_start_time = system_clock::from_time_t(mktime(&t));
            t.tm_hour = 13;
            t.tm_min = 0;
            _break_end_time = system_clock::from_time_t(mktime(&t));
            t.tm_hour = 15;
            if (_business_type == BusinessType::REPO) {
                t.tm_min = 30;
            }
            _close_time = system_clock::from_time_t(mktime(&t));
        } else {
            t.tm_hour = 1;
            t.tm_min = 30;
            t.tm_sec = 0;
            _open_time = system_clock::from_time_t(mktime(&t));
            t.tm_hour = 11;
            _break_start_time = system_clock::from_time_t(mktime(&t));
            t.tm_hour = 13;
            t.tm_min = 0;
            _break_end_time = system_clock::from_time_t(mktime(&t));
            t.tm_hour = 22;
            _close_time = system_clock::from_time_t(mktime(&t));
        }
        _status = Status::READY;
        _amount = 0.0;
        _left_amount = 0.0;
        _this_party_best_price = 0.0;
        _other_party_best_price = 0.0;
        _ticker_trade_price = 0.0;
        _traded_order_quantity = 0;
        _hot_order_quantity = 0;
        _cold_order_quantity = 0;
        _last_price = 0.0;
        _market_turnover = 0.0;
        _market_qty = 0;
        _traded_amount = 0.0;
        _traded_amount_with_fee = 0.0;
        _repo_traded_ticker_value = 0.0;
        _placed_qty = 0;
        _pending_qty = 0;
        _pending_amount = 0.0;
        _last_withdraw_qty = 0;
        _withdraw_qty = 0;
        _minute_report_uid = 0;
        _task_report_uid = 0;
        _order_report_uid = 0;
        _trade_report_uid = 0;
        _order_num = 0;
        _trade_num = 0;
        _refer_report_freq = 20s;
        _refer_cancel_freq = 60s;
        _remark = "";
        _task_destory_time = string_to_time_point("23:00:00");
        _update_status = Status::READY;
        _update_order_num = 0;
        _update_trade_num = 0;
        _progress = 0.0;
        _init_market_qty = 0;
        _init_market_turnover = 0;
        _first_order_flag = true;
        _maximal_qty = 1000000;
        _businessSideType = BusinessSideType::UNKNOWN;
        _error_code = {};

        if (_business_type == BusinessType::REPO) {
            if (_quantity.second == SideType::SELL) {
                _businessSideType = BusinessSideType::REPO_SELL;
            }
        }

        if (_business_type == BusinessType::CASH) {
            if (_quantity.second == SideType::BUY) {
                _businessSideType = BusinessSideType::CASH_BUY;
            } else if (_quantity.second == SideType::SELL) {
                _businessSideType = BusinessSideType::CASH_SELL;
            }

        }

        if (auto owner = _owner.lock()) {
            RedisServer::instance().on_task_created(_task_id, owner->get_username(), _ticker, _quantity);
            owner->on_task_id(_task_id, _task_user_id);
        }

    }

    Task::~Task() {
        LOG(INFO) << "start exit task";
        LOG(INFO) << "task exit successfully";
    }

    void Task::start(const json &params) {
        RedisServer::instance().on_task_status_changed(_task_id, Status::IN_PROGRESS);
    }


    int32_t Task::pause(const std::string &cli_session_id) {
        lock_guard<mutex> guard(_mutex);
        if (_status == Status::IN_PROGRESS or _status == Status::READY or _status == Status::CAPITAL_INSUFFICIENT_ERROR
            or _status == Status::SECURITY_INSUFFICIENT_ERROR) {
            _task_status_change_no_report(Status::PAUSED, cli_session_id);
            return 0;
        }
        if (_status == Status::PAUSED) {
            return 1;
        }
        return 2;
    }

    int32_t Task::resume(const std::string &cli_session_id) {
        lock_guard<mutex> guard(_mutex);
        if (_status == Status::PAUSED) {
            _task_status_change_no_report(Status::IN_PROGRESS, cli_session_id);
            return 0;
        }
        if (_status == Status::IN_PROGRESS) {
            return 1;
        }
        return 2;
    }

    int32_t Task::stop(const std::string &cli_session_id) {
        if (_status == Status::IN_PROGRESS or _status == Status::PAUSED or _status == Status::READY or
            _status == Status::CAPITAL_INSUFFICIENT_ERROR or _status == Status::SECURITY_INSUFFICIENT_ERROR) {
            on_task_stop(cli_session_id);
            return 0;
        }
        if (_status == Status::STOPPED) {
            return 1;
        }
        return 2;
    }

    int32_t Task::stop() {
        return stop("");
    }


    bool Task::_update_asset(double trade_amount_with_fee, const ats::SideType &side,
                             const ats::BusinessType &business_type) {
        if (auto owner = _owner.lock()) {
            if (owner->get_user_asset().first->buying_power > trade_amount_with_fee) {
                owner->update_buying_power(trade_amount_with_fee * (-1));

                return true;
            }
        }
        return false;
    }

    bool Task::_update_position(int64_t quantity, const std::string &ticker_id) {

        if (auto owner = _owner.lock()) {
            for (auto &stock_position : owner->get_stock_positions()) {
                if (stock_position.first->ticker == ticker_id) {
                    auto left_qty = stock_position.first->sellable_qty - quantity;
                    if (left_qty >= 0) {
                        owner->update_sellable_position(quantity * (-1), stock_position.first);
                        return true;
                    }
                    return false;
                }
            }
        }
        return false;
    }

    void Task::_update_asset_position_left_amount(const std::shared_ptr<ats::OrderInfo> &orderInfo) {
        if (auto owner = _owner.lock()) {
            if (owner->query_asset()) {
                double buying_poweer_add = 0.0;
                double withdraw_amount = 0.0;
                _asset_position_calculator->update_asset(orderInfo, buying_poweer_add, withdraw_amount);
                owner->update_buying_power(buying_poweer_add);
                if (_amount_flag) {
                    _left_amount += withdraw_amount;
                }
            }
            if (orderInfo->business_type == BusinessType::CASH and orderInfo->side == SideType::SELL) {
                if (owner->query_stock_position(orderInfo->ticker)) {
                    int64_t position_cost = 0;
                    auto new_stk_position = owner->query_stock_position(orderInfo->ticker);
                    _asset_position_calculator->update_position(orderInfo->qty_left, orderInfo->side, position_cost);
                    owner->update_sellable_position(position_cost, new_stk_position);
                }
            }
        }
    }

    void Task::on_order_info(shared_ptr<OrderInfo> order_info, shared_ptr<ErrorInfo> error_info) {
        lock_guard<mutex> guard(_mutex);
        _update_asset_position_left_amount(order_info);
        auto &order = _xtp_order_id_orders[order_info->order_xtp_id];
        order->on_order_info(order_info, move(error_info));
        LOG(INFO) << "order info update | " << order_info->order_xtp_id;
    }

    void Task::on_trade_report(shared_ptr<TradeReport> trade_report) {
        lock_guard<mutex> guard(_mutex);
        auto &order = _xtp_order_id_orders[trade_report->order_xtp_id];
        order->on_trade_report(trade_report);
    }


    void Task::update_market_data(unique_ptr<MarketData> market_data) {
        gettimeofday(&_t_quote, nullptr);
        if (_quantity.second == SideType::BUY) {
            _this_party_best_price = market_data->bid[0];
            _other_party_best_price = market_data->ask[0];
        } else {
            _this_party_best_price = market_data->ask[0];
            _other_party_best_price = market_data->bid[0];
        }
        if (_this_party_best_price == 0) {
            _this_party_best_price = _other_party_best_price;
        }
        if (_other_party_best_price == 0) {
            _other_party_best_price = _this_party_best_price;
        }
        if (_this_party_best_price == 0) {
            _error_code = ErrorCategory(ErrorCode::CANNOT_GET_TICKER_QUOTE);
            {
                std::lock_guard<std::mutex> guard(_remark_mutex);
                _remark = _error_code.message();
            }
            LOG(ERROR) << "this party best price: " << _this_party_best_price << " | other party best price: "
                       << _other_party_best_price;
            LOG(ERROR) << _remark << " | task_id: " << _task_id;
            on_task_stop();
        }
        if (_init_market_qty == 0 or _init_market_turnover == 0) {
            _init_market_qty = market_data->qty;
            _init_market_turnover = market_data->turnover;
        }
        _market_qty = market_data->qty - _init_market_qty;
        _market_turnover = market_data->turnover - _init_market_turnover;
        _last_price = market_data->last_price;
        _current_market_data = move(market_data);
        if (_amount_flag and _other_party_best_price > 0) {
            auto current_qty = _asset_position_calculator->update_quantity_with_amount(_quantity, _ticker_trade_price,
                                                                                       _left_amount, _placed_qty,
                                                                                       _other_party_best_price);
            int minimal_qty = _asset_position_calculator->get_minimal_qty();
            lock_guard<mutex> guard(_mutex);

            _minimal_trade_quantity = (int64_t) (ceil(_minimal_trade_amount / _ticker_trade_price / minimal_qty) *
                                                 minimal_qty);
            _quantity.first = max(current_qty, _placed_qty);
        }
    }

    bool Task::update_asset_and_stock_position() {
        LOG(INFO) << "start to update asset and stock position | task id: " << _task_id;
        if (auto owner = _owner.lock()) {
            if (_businessSideType == BusinessSideType::CASH_BUY or _businessSideType == BusinessSideType::REPO_SELL) {
                if (auto asset = owner->query_asset()) {
                    auto tradable_capital = asset->buying_power;
                    LOG(INFO) << "tradable captical: " << tradable_capital << " | task id: " << _task_id;
                    return true;
                }
                LOG(ERROR) << "query captical failed | task id: " << _task_id;
            } else if (_businessSideType == BusinessSideType::CASH_SELL) {
                if (auto stk_position = owner->query_stock_position(_ticker.first)) {
                    if (auto asset = owner->query_asset()) {
                        auto tradable_security = stk_position->sellable_qty;
                        auto tradable_capital = asset->buying_power;
                        LOG(INFO) << "tradable captical: " << tradable_capital << " | task id: " << _task_id;
                        LOG(INFO) << "sellable security: " << tradable_security << " | task id: " << _task_id;
                        return true;
                    }
                    LOG(ERROR) << "query captical failed | task id: " << _task_id;
                }
                LOG(ERROR) << "query security failed | task id: " << _task_id;
            }
        }
        return false;
    }

    void Task::update_order_status() {
        _traded_order_quantity = 0;
        _hot_order_quantity = 0;
        _cold_order_quantity = 0;
        _order_num = 0;
        _trade_num = 0;
        _traded_amount = 0;
        _repo_traded_ticker_value = 0.0;
        _traded_amount_with_fee = 0.0;
        _pending_amount_with_fee = 0.0;
        _withdraw_qty = 0;
        _pending_qty = 0;
        _pending_amount = 0;
        _left_order_xtp_ids.clear();
        for (const auto &order : _xtp_order_id_orders) {
            double order_price = order.second->get_price().first;
            int64_t traded_qty = order.second->get_traded_quantity();
            int64_t left_qty = order.second->get_left_quantity();
            double traded_amount = order.second->get_traded_amount();
            double repo_traded_ticker_value = order.second->get_repo_traded_ticker_value();
            size_t order_num = order.second->get_order_num();
            size_t trade_num = order.second->get_trade_num();
            if (get<0>(order.second->get_left_order()) > 0) {
                lock_guard<mutex> guard(_mutex);
                _left_order_xtp_ids.push_back(order.second->get_left_order());
            }
            {
                lock_guard<mutex> guard(_mutex);
                _order_num += order_num;
                _trade_num += trade_num;
                _traded_order_quantity += traded_qty;
                _traded_amount += traded_amount;
                _repo_traded_ticker_value += repo_traded_ticker_value;

                if (traded_amount > 0.0) {
                    _traded_amount_with_fee += _asset_position_calculator->amount_with_fee(traded_amount);
                }
                _withdraw_qty += order.second->get_withdraw_quantity();
                _pending_qty += order.second->get_left_quantity();
                auto pending_amount = order.second->get_left_amount();
                _pending_amount += pending_amount;
                if (pending_amount > 0.0) {
                    _pending_amount_with_fee += _asset_position_calculator->amount_with_fee(pending_amount);
                }
                if (_quantity.second == SideType::BUY) {
                    if (order_price >= _this_party_best_price) {
                        _hot_order_quantity += left_qty;
                    } else {
                        _cold_order_quantity += left_qty;
                    }
                } else {
                    if (order_price <= _this_party_best_price) {
                        _hot_order_quantity += left_qty;
                    } else {
                        _cold_order_quantity += left_qty;
                    }
                }
            }
            // TODO stop if break the rules
            auto error_msg = order.second->get_error_info();
            if (!error_msg.empty()) {
                _remark = error_msg;
                LOG(ERROR) << "error msg: " << error_msg << " | task id: " << _task_id;
                task_status_change(_task_id, Status::STOPPED);
            }
        }
        LOG(INFO) << "order num: " << _order_num << " | task id: " << _task_id;
        LOG(INFO) << "trade num: " << _trade_num << " | task id: " << _task_id;
        LOG(INFO) << "traded vol: " << _traded_order_quantity << " | task id: " << _task_id;
        LOG(INFO) << "traded cost amount: " << _traded_amount_with_fee << " | task id: " << _task_id;
        LOG(INFO) << "withdraw qty: " << _withdraw_qty << " | task id: " << _task_id;
        LOG(INFO) << "pending qty: " << _pending_qty << " | task id: " << _task_id;
        LOG(INFO) << "pending amount: " << _pending_amount << " | task id: " << _task_id;
        LOG(INFO) << "pending cost amount: " << _pending_amount_with_fee << " | task id: " << _task_id;
        LOG(INFO) << "remark: " << _remark << " | task id: " << _task_id;
        _update_placed_qty();
        if (_traded_order_quantity >= _quantity.first) {
            cancel_all_orders();
            task_status_change(_task_id, Status::FINISHED);
        } else if (_amount_flag and abs(_traded_amount_with_fee) >= _amount) {
            cancel_all_orders();
            task_status_change(_task_id, Status::FINISHED);
        } else if (_status == Status::STOPPED) {
            cancel_all_orders();
        }
    }

    void Task::_update_placed_qty() {
        _withdraw_qty -= _last_withdraw_qty;
        if (_withdraw_qty > 0) {
            _placed_qty -= _withdraw_qty;
            _last_withdraw_qty += _withdraw_qty;
            _withdraw_qty = 0;
        }
        LOG(WARNING) << "actual placed qty: " << _placed_qty << " | task id: " << _task_id;
    }

    bool Task::place_order(double original_price, int64_t quantity) {

        LOG(INFO) << "start to place order | task id " << _task_id;
        try {
            if (0 == quantity or 0 == original_price) {
                return false;
            }
            double price = _asset_position_calculator->get_correct_price(original_price);
            double trade_price = _asset_position_calculator->get_correct_price(price);
            double trade_fee = _asset_position_calculator->amount_fee(trade_price * quantity);
            double trade_amount_with_fee = _asset_position_calculator->amount_with_fee(trade_price * quantity);
            auto price_ = make_pair(price, PriceType::LIMIT);
            double traded_qty = 0;
            bool saleable_flag = false;
            int64_t num = quantity / _maximal_qty + (quantity % _maximal_qty > 0 ? 1 : 0);
            if (_business_type == BusinessType::CASH and _quantity.second == SideType::SELL) {
                saleable_flag = true;
                trade_fee = 0;
            }

            if (auto owner = _owner.lock()) {

                if (saleable_flag and (!_update_position(quantity, _ticker.first))) {
                    {
                        lock_guard<mutex> guard(_remark_mutex);
                        auto errorCode = ErrorCategory(ErrorCode::CANNOT_FIND_SELLABLE_QUANTITY);
                        _remark = errorCode.message();
                    }
                    if (_status != Status::SECURITY_INSUFFICIENT_ERROR) {
                        task_status_change(_task_id, Status::SECURITY_INSUFFICIENT_ERROR);
                    }
                    return false;
                }


                if ((!saleable_flag) and (!_update_asset(trade_amount_with_fee, _quantity.second, _business_type))) {
                    {
                        lock_guard<mutex> guard(_remark_mutex);
                        auto errorCode = ErrorCategory(ErrorCode::CANNOT_FIND_TRADABLE_ASSET);
                        _remark = errorCode.message();

                    }
                    if (_status != Status::CAPITAL_INSUFFICIENT_ERROR) {
                        task_status_change(_task_id, Status::CAPITAL_INSUFFICIENT_ERROR);
                    }
                    return false;
                }
                if (_amount_flag) {
                    lock_guard<mutex> guard(_mutex);
                    _left_amount -= (trade_fee + price * quantity);
                }
                for (int i = 0; i < num; i++) {
                    auto qty_origin = i < num - 1 ? (((double) quantity / num) * (i + 1) - traded_qty) /
                                             _asset_position_calculator->get_minimal_qty() *
                                             _asset_position_calculator->get_minimal_qty() :
                               quantity - traded_qty;
                    uint64_t qty = _asset_position_calculator->quantity_integer((uint64_t )qty_origin);
                    if (qty <= 0) {
                        return false;
                    }

                    auto qty_ = make_pair(qty, _quantity.second);
                    auto xtp_order_id = owner->place_order(_task_id, _ticker, price_, qty_, _business_type);
                    if (xtp_order_id == 0) {
                        LOG(ERROR) << "Place Order Failed ";
                        continue;
                    }
                    traded_qty += qty;
                    _placed_qty += (int64_t) qty;
                    lock_guard<mutex> guard(_mutex);
                    _xtp_order_id_orders[xtp_order_id] = make_unique<Order>(xtp_order_id, _ticker, price_, qty_);
                    long long time_point = 1000000 * _t_quote.tv_sec + _t_quote.tv_usec;
                    _t_quotes[xtp_order_id] = time_point;
                    LOG(INFO) << "place successfully" << " xtp order id: " << xtp_order_id << " | task id: "
                              << _task_id;
                }
                if (_status == Status::CAPITAL_INSUFFICIENT_ERROR or _status == Status::SECURITY_INSUFFICIENT_ERROR) {
                    task_status_change(_task_id, Status::IN_PROGRESS);
                    _error_code = ErrorCategory(ErrorCode::NO_ERROR);
                    {
                        std::lock_guard<std::mutex> guard(_remark_mutex);
                        _remark = "";
                    }
                }
                return true;
            }
            return false;

        } catch (exception &e) {
            LOG(ERROR) << "UNKNOW_ERROR" << "|" << e.what();
            return false;
        }
    }

    pair<PriceAction, shared_ptr<MarketQuotationData>> Task::find_market_quotation(uint64_t xtp_order_id) {
        auto iter = _xtp_order_id_market_data.find(xtp_order_id);
        if (iter != _xtp_order_id_market_data.end()) {
            return iter->second;
        } else {
            LOG(ERROR) << "can not found right order id in market Quotation Data | task_id" << _task_id;
            return make_pair(PriceAction::UNKNOWN, nullptr);
        }
    }

    bool Task::cancel_order(uint64_t xtp_order_id) {
        LOG(INFO) << "start to cancel order | xtp order id: " << xtp_order_id << " | task id " << _task_id;
        if (auto owner = _owner.lock()) {
            auto iter = find_if(_left_order_xtp_ids.begin(), _left_order_xtp_ids.end(),
                                [&xtp_order_id](tuple<uint64_t, int64_t, double> left_order_xtp_id) {
                                    return get<0>(left_order_xtp_id) == xtp_order_id;
                                });
            if (iter != _left_order_xtp_ids.end()) {
                owner->cancel_order(xtp_order_id);
                _left_order_xtp_ids.erase(iter);
                LOG(INFO) << "cancel order finished | xtp order id: " << xtp_order_id << " | task id " << _task_id;
            } else {
                LOG(WARNING) << "xtp order id not found | xtp_order_id: " << xtp_order_id << " | task id " << _task_id;
            }
            return true;
        }
        LOG(ERROR) << "owner expired" << " | task id: " << _task_id;
        return false;
    }

    void Task::cancel_cold_orders() {
        auto left_order_xtp_ids(_left_order_xtp_ids);
        LOG(INFO) << "start to cancel cold orders | task id " << _task_id;
        for (auto &order_xtp_id : left_order_xtp_ids) {
            auto xtp_id = get<0>(order_xtp_id);
            auto price = get<2>(order_xtp_id);
            auto cold_order_flag = false;
            if (price < _this_party_best_price and _quantity.second == SideType::BUY) {
                cold_order_flag = true;
                LOG(INFO) << "current price: " << price << " | best buy price: " << _this_party_best_price
                          << " | task id: " << _task_id;
            } else if (price > _this_party_best_price and _quantity.second == SideType::SELL) {
                LOG(INFO) << "current price: " << price << " | best sell price: " << _this_party_best_price
                          << " | task id: " << _task_id;
                cold_order_flag = true;
            }
            if (xtp_id != 0 and cold_order_flag) {
                LOG(INFO) << "cancel pending order: " << xtp_id;
                cancel_order(xtp_id);
            }
        }
        LOG(INFO) << "cancel cold orders finished | task id " << _task_id;
    }

    void Task::cancel_all_orders() {
        auto left_order_xtp_ids(_left_order_xtp_ids);
        unsigned long xtp_id = 0;
        LOG(INFO) << "start to cancel all order | task id " << _task_id;
        for (auto &order_xtp_id : left_order_xtp_ids) {
            xtp_id = get<0>(order_xtp_id);
            if (xtp_id != 0) {
                cancel_order(xtp_id);
            }
        }
        _left_order_xtp_ids.clear();
        LOG(INFO) << "cancel all order finished | task id " << _task_id;
    }

    pair<uint64_t, int64_t> Task::find_worst_order() {
        LOG(INFO) << "start to find worst order | task id " << _task_id;
        uint64_t worst_xtp_order_id = 0;
        int64_t worst_qty = 0;
        double worst_price = _quantity.second == SideType::BUY ? 9999.0 : 0.0;
        lock_guard<mutex> guard(_mutex);
        for (auto &order : _xtp_order_id_orders) {
            auto xtp_order_id = order.second->get_xtp_order_id();
            auto iter = find_if(_left_order_xtp_ids.begin(), _left_order_xtp_ids.end(),
                                [&xtp_order_id](tuple<uint64_t, int64_t, double> left_order_xtp_id) {
                                    return get<0>(left_order_xtp_id) == xtp_order_id;
                                });
            if (iter != _left_order_xtp_ids.end()) {
                auto price = order.second->get_price().first;
                if ((_quantity.second == SideType::BUY and price < worst_price) or
                    (_quantity.second == SideType::SELL and price > worst_price)) {
                    worst_xtp_order_id = order.second->get_xtp_order_id();
                    worst_qty = order.second->get_left_quantity();
                    worst_price = price;
                } else if (price == worst_price and order.second->get_left_quantity() > worst_qty) {
                    worst_xtp_order_id = order.second->get_xtp_order_id();
                    worst_qty = order.second->get_left_quantity();
                }
            }
        }
        return make_pair(worst_xtp_order_id, worst_qty);
    }

    void Task::parameter_parse() {
        LOG(INFO) << "start to parse time params | task id " << _task_id;
        _start_time = _input_start_time;
        _end_time = _input_end_time;
        if (system_clock::now() > _start_time)
            _start_time = system_clock::now();
        if (_start_time < _open_time)
            _start_time = _open_time;
        if (_end_time > _break_start_time and _end_time <= _break_end_time)
            _end_time = _break_start_time;
        if (_start_time >= _break_start_time and _start_time < _break_end_time)
            _start_time = _break_end_time;
        if (system_clock::now() > _end_time or
            _start_time >= _end_time or
            _end_time < _open_time or
            _start_time >= _close_time or
            _end_time > _close_time or
            (calc_time_span(_end_time) - calc_time_span(_start_time) < 5min)) {
            _error_code = ErrorCategory(ErrorCode::INVALID_TIME_PARAMETER);
            {
                std::lock_guard<std::mutex> guard(_remark_mutex);
                _remark = _error_code.message();
            }
            task_status_change(_task_id, Status::STOPPED);
            LOG(ERROR) << "time parameters error | task id: " << _task_id << "start_time: " <<
                       time_point_to_string(_start_time) << " end_time: " << time_point_to_string(_end_time);
        }
        LOG(INFO) << "Check some detail of order";
        check_detail();
        check_time_out(_end_time);
        LOG(INFO) << "parse params finished | task id " << _task_id;
    }

    void Task::check_detail() {
        if (_business_type == BusinessType::REPO) {
            if (!(_quantity.second == SideType::SELL)) {
                LOG(ERROR) << "wrong SideType of Repo, The right one is SELL" << _task_id;
                _error_code = ErrorCategory(ErrorCode::INVALID_SIDE_TYPE_PARAMETER);
                {
                    std::lock_guard<std::mutex> guard(_remark_mutex);
                    _remark = _error_code.message();
                }
                on_task_stop();
            }
        }
    }

    void Task::stop_and_set_msg(std::string remark_msg) {
        {
            std::lock_guard<std::mutex> guard(_remark_mutex);
            _remark = move(remark_msg);
        }
        on_task_stop();
    }

    void Task::task_init() {
        LOG(INFO) << "start to task init | task id: " << _task_id;
        task_status_change(_task_id, _status);
        while (system_clock::now() < _start_time) {
            this_thread::sleep_for(1s);
            check_time_out(_end_time);

            if (_status == Status::STOPPED) {
                LOG(INFO) << "task stopped in task init | task id " << _task_id;
                break;
            }
            if (_status == Status::IN_PROGRESS) {
                task_status_change(_task_id, Status::READY);
            }
        }
        LOG(INFO) << "ready to start task | task id: " << _task_id;
        check_time_out(_end_time);

        if (_status == Status::READY) {
            task_status_change(_task_id, Status::IN_PROGRESS);
        }
        if (_status == Status::IN_PROGRESS) {
            market_data_init();
            _left_amount = _amount;

        }
        _last_time = _start_time;
    }

    void Task::on_minute_report() {
        if (auto user = _owner.lock()) {
            auto minute_report = make_shared<MinuteReport>(_task_id, get_type(), _minute_report_uid++);
            minute_report->market_price = _last_price;
            if (_business_type == BusinessType::REPO) {
                if (_traded_order_quantity > 0) {
                    minute_report->avg_traded_price =
                            round(_repo_traded_ticker_value / _traded_order_quantity * 10000) / 10000.0;
                } else {
                    minute_report->avg_traded_price = 0.0;
                }
                minute_report->avg_market_price = 0.0;
            } else {
                if (_traded_order_quantity > 0) {
                    minute_report->avg_traded_price =
                            round(_traded_amount / (_traded_order_quantity) * 10000) / 10000.0;
                } else {
                    minute_report->avg_traded_price = 0.0;
                }
                if (_market_qty > 0) {
                    minute_report->avg_market_price = round(_market_turnover / _market_qty * 10000) / 10000.0;
                } else {
                    minute_report->avg_market_price = 0.0;
                }
            }
            minute_report->task_traded_quantity = _traded_order_quantity;
            minute_report->market_traded_quantity = _market_qty;
            minute_report->trade_ratio = (_traded_order_quantity + 0.0) / _quantity.first;
            minute_report->trade_time = time_point_to_string(std::chrono::system_clock::now());
            user->on_minute_report(_task_id, get_type(), minute_report);
        }
        LOG(INFO) << "send minute report | task id " << _task_id;
    }

    void Task::on_task_report(system_clock::time_point start_time, system_clock::time_point end_time,
                              const string &cli_session_id) {
        string user_mac = {};
        if (empty(cli_session_id)) {
            user_mac = _user_mac;
        } else {
            user_mac = _find_mac(cli_session_id);
        }
        if (auto user = _owner.lock()) {
            auto task_report = make_shared<TaskReport>(_task_id, get_type(), _task_report_uid++);
            task_report->ticker = _ticker.first;
            task_report->quantity = _quantity.first;
            task_report->amount = _amount;
            task_report->side = _quantity.second;
            task_report->start_time = start_time;
            task_report->end_time = end_time;
            task_report->traded_quantity = _traded_order_quantity;
            task_report->pending_quantity = _pending_qty;
            task_report->order_num = _order_num;
            task_report->trade_num = _trade_num;
            task_report->progress = (_traded_order_quantity + 0.0) / _quantity.first;
            task_report->traded_amount = _traded_amount;
            task_report->pending_amount = _pending_amount;
            task_report->predicted_amount =
                    _traded_amount + (_quantity.first - _traded_order_quantity) * _this_party_best_price;
            if (_business_type == BusinessType::REPO) {
                if (_traded_order_quantity > 0) {
                    task_report->avg_traded_price =
                            round(_repo_traded_ticker_value / _traded_order_quantity * 10000) / 10000.0;
                } else {
                    task_report->avg_traded_price = 0.0;
                }
                task_report->avg_market_price = 0.0;
                task_report->bps = 0.0;
            } else {
                if (_traded_order_quantity > 0) {
                    task_report->avg_traded_price = round(_traded_amount / _traded_order_quantity * 10000) / 10000.0;
                } else {
                    task_report->avg_traded_price = 0.0;
                    task_report->bps = 0.0;
                }
                if (_market_qty > 0) {
                    task_report->avg_market_price = round(_market_turnover / _market_qty * 10000) / 10000.0;
                    if (task_report->avg_traded_price > 0 and task_report->avg_market_price > 0) {
                        task_report->bps =
                                (task_report->avg_traded_price - task_report->avg_market_price) /
                                (task_report->avg_market_price) *
                                10000 * (_quantity.second == SideType::BUY ? -1 : 1);
                    }
                } else {
                    task_report->avg_market_price = 0.0;
                    task_report->bps = 0.0;
                }
            }
            task_report->left_amount = _left_amount;
            task_report->left_quantity = _quantity.first - _traded_order_quantity - _pending_qty;
            task_report->passive_ratio = 0.0;
            task_report->remark = _remark;
            task_report->status = status_to_string(_status);
            user->on_task_report(_task_id, get_type(), task_report, user_mac);
        }
        LOG(INFO) << "send task report | task id " << _task_id;
    }

    void Task::on_task_stop(const string &cli_session_id) {
        LOG(ERROR) << "on task stopped | task id: " << _task_id;
        cancel_all_orders();
        _task_status_change_no_report(Status::STOPPED, cli_session_id);

    }

    bool Task::on_task_update() {
        lock_guard<mutex> guard(_mutex);
        if ((_update_status != _status) || ((_traded_order_quantity + 0.0) / _quantity.first > _progress) ||
            (_order_num != _update_order_num) || (_trade_num != _update_trade_num)) {
            _update_status = _status;
            _update_order_num = _order_num;
            _update_trade_num = _trade_num;
            _progress = (_traded_order_quantity + 0.0) / _quantity.first;
            LOG(INFO) << "task status updated | task id " << _task_id;
            return true;
        }
        return false;
    }

    void Task::task_status_change(uint32_t task_id, Status task_status, const std::string &cli_session_id) {
        std::lock_guard<std::mutex> guard(_report_update_mutex);
        RedisServer::instance().on_task_status_changed(task_id, task_status);
        _status = task_status;
        if (empty(cli_session_id)) {
            on_task_report(_input_start_time, _input_end_time);
        } else {
            on_task_report(_input_start_time, _input_end_time, cli_session_id);
        }

    }

    void Task::_task_status_change_no_report(Status task_status, const string &cli_session_id) {
        std::lock_guard<std::mutex> guard(_report_update_mutex);
        if(!empty(cli_session_id)) {
            _user_mac = _find_mac(cli_session_id);
        }
        _status = task_status;
        RedisServer::instance().on_task_status_changed(_task_id, _status);
    }


    bool Task::on_minute_update(system_clock::duration &minute_update_time) {
        auto minute_span = calc_time_span(std::chrono::system_clock::now()) - minute_update_time;
        return minute_span >= _refer_report_freq;
    }

    void Task::market_data_init() {
        LOG(INFO) << "start to subscribed market data | task id: " << _task_id;
        QuoteServer::instance().subscribe_market_data(_ticker);
    }

    int Task::check_time_out(system_clock::time_point end_time) {
        auto time_spread = calc_time_span(end_time) - calc_time_span(system_clock::now());
        if (time_spread.count() <= 0) {
            LOG(WARNING) << "Time Out" << " | task id: " << _task_id;
            _error_code = ErrorCategory(ErrorCode::TIME_OUT);
            {
                std::lock_guard<std::mutex> guard(_remark_mutex);
                _remark = _error_code.message();
            }
            on_task_stop();
            return -1;
        }
        return 1;
    }

    system_clock::duration Task::calc_time_span(system_clock::time_point tp) const {
        // TODO: For open test
        if (Configuration::instance().get_trade_time_limit()) {
            if (tp < _open_time) {
                return 0h;
            }
            if (tp < _break_start_time) {
                return tp - _open_time;
            }
            if (tp < _break_end_time) {
                return 2h;
            }
            if (tp < _close_time) {
                return tp - _break_end_time + 2h;
            }
            if (_business_type == BusinessType::REPO) {
                return 270min;
            }
            return 4h;
        } else {
            if (tp < _open_time) {
                return 0h;
            }
            if (tp < _break_start_time) {
                return tp - _open_time;
            }
            if (tp < _break_end_time) {
                return 10h;
            }
            return tp - _break_end_time + 10h;
        }
    }

    bool Task::set_xtp_order_id_market_data(ats::SideType side, double price, unsigned long xtp_order_id,
                                            std::shared_ptr<MarketQuotationData> marketquotationdata) {
        _xtp_order_id_market_data[xtp_order_id] = make_pair(marketquotationdata->judge_active_passive_price(
                side, price), marketquotationdata);
        return true;

    }

    bool Task::_break_time_check() {
        if (system_clock::now() >= _break_start_time and
            system_clock::now() < _break_end_time) {
            this_thread::sleep_for(1s);
            return true;

        }
        return false;
    }

    string Task::_find_mac(const string &cli_session_id) {
        if (auto user = _owner.lock()) {
            return user->find_mac(cli_session_id);

        }
        LOG(ERROR) << "Illegal session_id" << endl;
        return "Illegal session_id";

    }


}

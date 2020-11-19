//
// Created by xtgu on 9/20/19.
//

#include "taskproxy.h"
#include "quoteserver.h"
#include "user.h"

namespace ats {
    using namespace std;

    TaskProxy::TaskProxy(std::shared_ptr<Task> newTask):_task(move(newTask)){
    }

    void TaskProxy::start(const nlohmann::json &params) {
        LOG(INFO) << "IN TaskProxy start" << "| task id :"<< _task->get_task_id()<< endl;
        _task->start(params);
        thread &&thread1 = thread(&TaskInterface::execution, this);
        _thread = move(thread1);
        _thread.detach();
    }

    int32_t TaskProxy::pause(const string &cli_session_id) {
        LOG(INFO) << "IN TaskProxy pause" << "| task id :"<< _task->get_task_id()<< endl;
        return _task->pause(cli_session_id);

    }

    int32_t TaskProxy::resume(const string &cli_session_id) {
        LOG(INFO) << "IN TaskProxy resume" << "| task id :"<< _task->get_task_id()<< endl;
        return _task->resume(cli_session_id);

    }

    int32_t TaskProxy::stop(const string &cli_session_id) {
        LOG(INFO) << "IN TaskProxy stop" << "| task id :"<< _task->get_task_id()<< endl;
        return _task->stop(cli_session_id);

    }

    int32_t TaskProxy::stop(){
        LOG(INFO) << "IN TaskProxy stop" << "| task id :"<< _task->get_task_id()<< endl;
        return _task->stop();
    }

    bool TaskProxy::execution() {
        LOG(INFO) << "IN TaskProxy execution" << "| task id :"<< _task->get_task_id() << endl;

        task_init();

        auto minute_update_time = calc_time_span(_task->_start_time);

        while (true) {
            update_order_status();
            check_time_out(_task->_end_time);
            if (_task->_status == Status::STOPPED or _task->_status == Status::FINISHED) {
                on_minute_report();
                LOG(INFO) << "task stopped or finished. traded vol is: " <<_task-> _traded_order_quantity << " | task id: "
                          << _task-> _task_id;
                break;
            }
            if (on_task_update()) {
                on_task_report(_task->_input_start_time, _task->_input_end_time);
            }
            if (on_minute_update(minute_update_time)) {
                minute_update_time = calc_time_span(std::chrono::system_clock::now());
                on_minute_report();
            }

            if (_task->_status == Status::PAUSED) {
                LOG(INFO) << "task paused. traded vol is: " << _task-> _traded_order_quantity << " | task id: " << _task->_task_id;
                this_thread::sleep_for(1s);
                continue;
            }

            if (_task->_break_time_check()) {
                continue;
            }

            if (auto market_data = QuoteServer::instance().query_market_data(_task->_ticker)) {
                update_market_data(move(market_data));
            } else {
                this_thread::sleep_for(1s);
                LOG(INFO) << "Fail to get market data" << "| ticker:" << _task->_ticker.first << " | task id: " << _task->_task_id;
                continue;
            }
            if (!update_asset_and_stock_position()) {
                LOG(ERROR) << "Fail to get stock position" << "| task id:" << _task->_task_id;
                this_thread::sleep_for(1s);
                continue;
            }

            if (!_task->execution()) {
                continue;
            }

        }
        this_thread::sleep_for(3s);
        update_order_status();
        on_minute_report();
        on_task_report(_task->_input_start_time, _task->_input_end_time);
        QuoteServer::instance().unsubscribe_market_data(_task->_ticker);
        this_thread::sleep_until(_task->_task_destory_time);
        if (auto owner = _task->_owner.lock()) {
            LOG(INFO) << "start to erase task id: " << _task->_task_id;
            owner->on_task_destroyed(_task->_task_id);
            owner->write_time_difference(_task->_task_id, _task->_t_quotes);
            LOG(WARNING) << "tasks id was destroyed: " << _task->_task_id;
        } else {
            LOG(ERROR) << "owner expired" << " | task id: " << _task->_task_id;
        }

        return true;

    }

    void TaskProxy::update_market_data(std::unique_ptr<MarketData> market_data) {
        LOG(INFO) << "IN TaskProxy update_market_data " << "| task id :"<< _task->get_task_id() << endl;
        _task->update_market_data(move(market_data));
    }

    Type TaskProxy::get_type() const {
        LOG(INFO) << "IN TaskProxy get_type " << "| task id :"<< _task->get_task_id() << endl;
        return _task->get_type();
    }

    uint32_t TaskProxy::get_task_id() const {
        return _task->get_task_id();
    }

    size_t TaskProxy::get_order_uid() {
        LOG(INFO) << "IN TaskProxy get_order_uid " << "| task id :"<< _task->get_task_id() << endl;
        return _task->get_order_uid();
    }

    size_t TaskProxy::get_trade_uid() {
        LOG(INFO) << "IN TaskProxy get_trade_id" << "| task id :"<< _task->get_task_id() << endl;
        return _task->get_trade_uid();
    }

    void TaskProxy::set_calculator(std::shared_ptr<AssetPositionCalculator> calculator) {
        LOG(INFO) << "IN TaskProxy set_calculator" << "| task id :"<< _task->get_task_id() << endl;
        return _task->set_calculator(move(calculator));
    }



    void TaskProxy::on_order_info(std::shared_ptr<OrderInfo> order_info, std::shared_ptr<ErrorInfo> error_info) {
        LOG(INFO) << "IN TaskProxy on_order_info" << "| task id :"<< _task->get_task_id() << endl;
        _task->on_order_info(move(order_info), move(error_info));
    }


    void TaskProxy::on_trade_report(std::shared_ptr<TradeReport> trade_report) {
        LOG(INFO) << "IN TaskProxy ont_trade_report" << "| task id :"<< _task->get_task_id() << endl;
        _task->on_trade_report(move(trade_report));
    }

    std::pair<ats::PriceAction, std::shared_ptr<ats::MarketQuotationData>>
    TaskProxy::find_market_quotation(uint64_t xtp_order_id) {
        LOG(INFO) << "IN TaskProxy find_market_quotation" << "| task id :"<< _task->get_task_id() << endl;
        return _task->find_market_quotation(xtp_order_id);
    }

    bool TaskProxy::update_asset_and_stock_position() {
        LOG(INFO) << "IN TaskProxy update_asset_and_stock_position" << "| task id :"<< _task->get_task_id() << endl;
        return _task->update_asset_and_stock_position();
    }

    void TaskProxy::update_order_status() {
        LOG(INFO) << "IN TaskProxy update_order_status" << "| task id :"<< _task->get_task_id() << endl;
        _task->update_order_status();
    }

    bool TaskProxy::place_order(double price, int64_t quantity) {
        LOG(INFO) << "IN TaskProxy place_order" << "| task id :"<< _task->get_task_id() << endl;
        return _task->place_order(price,quantity);
    }

    void TaskProxy::parameter_parse() {
        LOG(INFO) << "IN TaskProxy parameter_parse" << "| task id :"<< _task->get_task_id() << endl;
        _task->parameter_parse();
    }

    void TaskProxy::task_init() {
        LOG(INFO) << "IN TaskProxy task_init" << "| task id :"<< _task->get_task_id() << endl;
        _task->task_init();
    }

    bool TaskProxy::on_task_update() {
        LOG(INFO) << "IN TaskProxy on_task_update" << "| task id :"<< _task->get_task_id() << endl;
        return _task->on_task_update();
    }

    void TaskProxy::on_task_report(chrono::system_clock::time_point start_time,
                                   std::chrono::system_clock::time_point end_time) {
        LOG(INFO) << "IN TaskProxy on_task_report" << "| task id :"<< _task->get_task_id() << endl;
        _task->on_task_report(start_time,end_time);
    }

    void TaskProxy::on_minute_report() {
        LOG(INFO) << "IN TaskProxy on_minute_report" << "| task id :"<< _task->get_task_id() << endl;
        _task->on_minute_report();
    }

    bool TaskProxy::on_minute_update(std::chrono::system_clock::duration &minute_update_time) {
        LOG(INFO) << "IN TaskProxy on_minute_update" << "| task id :"<< _task->get_task_id() << endl;
        return _task->on_minute_update(minute_update_time);
    }


    int TaskProxy::check_time_out(std::chrono::system_clock::time_point end_time){
        LOG(INFO) << "IN TaskProxy check_time_out" << "| task id :"<< _task->get_task_id() << endl;
        return _task->check_time_out(end_time);

    }

    std::chrono::system_clock::duration TaskProxy::calc_time_span(std::chrono::system_clock::time_point tp) const {
        LOG(INFO) << "IN TaskProxy calc_time_span" << "| task id :"<< _task->get_task_id() << endl;
        return _task->calc_time_span(tp);
    }

    bool TaskProxy::set_xtp_order_id_market_data(ats::SideType side, double price,unsigned long xtp_order_id,
            std::shared_ptr<ats::MarketQuotationData> makequotationdata) {
        LOG(INFO) << "IN set_Xtp_order_id_market_data" << "| task id :"<< _task->get_task_id() << endl;
        return _task->set_xtp_order_id_market_data(side,price,xtp_order_id, makequotationdata);
    }



}
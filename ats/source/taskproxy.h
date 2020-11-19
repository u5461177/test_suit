//
// Created by xtgu on 9/20/19.
//

#ifndef MUSE_TASKPROXY_H
#define MUSE_TASKPROXY_H

#include "taskinterface.h"
#include "task.h"


namespace ats {
    class TaskProxy : public TaskInterface {
    public:
        explicit TaskProxy(std::shared_ptr<ats::Task> newTask);

        ~TaskProxy() override = default;

    private:
        std::shared_ptr<Task> _task;
        std::thread _thread;

    public:
        void start(const nlohmann::json &params) override;

        bool execution() override;


        void update_market_data(std::unique_ptr<MarketData> market_data);

        Type get_type() const final;

        uint32_t get_task_id() const final;

        size_t get_order_uid() final;

        size_t get_trade_uid() final;


        void set_calculator(std::shared_ptr<AssetPositionCalculator> calculator) final;

        int32_t pause(const std::string &cli_session_id) override;

        int32_t resume(const std::string &cli_session_id) override;

        int32_t stop(const std::string &cli_session_id) override;

        int32_t stop() override;

        void on_order_info(std::shared_ptr<OrderInfo> order_info, std::shared_ptr<ErrorInfo> error_info) final;


        void on_trade_report(std::shared_ptr<TradeReport> trade_report) final;

        std::pair<ats::PriceAction, std::shared_ptr<ats::MarketQuotationData>>
        find_market_quotation(uint64_t xtp_order_id) final;

        bool update_asset_and_stock_position();

        void update_order_status();

        bool place_order(double price, int64_t quantity);


        void parameter_parse();

        void task_init();


        bool on_task_update();

        void on_task_report(std::chrono::system_clock::time_point start_time,
                            std::chrono::system_clock::time_point end_time);

        void on_minute_report();

        bool on_minute_update(std::chrono::system_clock::duration &minute_update_time);


        int check_time_out(std::chrono::system_clock::time_point end_time) override;


        std::chrono::system_clock::duration calc_time_span(std::chrono::system_clock::time_point tp) const;


        bool set_xtp_order_id_market_data(ats::SideType side, double price, unsigned long xtp_order_id,
                                          std::shared_ptr<ats::MarketQuotationData> marketquotationdata) final;

        std::shared_ptr<ats::MarketData> get_current_market_data() const final;

        void stop_and_set_msg(std::string remark_msg) final;

    };

    inline std::shared_ptr<ats::MarketData> TaskProxy::get_current_market_data() const {
        return _task->get_current_market_data();
    }

    inline void TaskProxy::stop_and_set_msg(std::string remark_msg) {
        _task->stop_and_set_msg(move(remark_msg));
    }


}


#endif //MUSE_TASKPROXY_H

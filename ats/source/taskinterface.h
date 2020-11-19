//
// Created by xtgu on 9/20/19.
//

#ifndef MUSE_TASKINTERFACE_H
#define MUSE_TASKINTERFACE_H

#include "json.hpp"
#include "assetpositioncalculator.h"
#include "marketquotationdata.h"

namespace ats {

    enum class Type {
        TWAP,
        VWAP,
        POV,
        IS,
        RANDOM,
        ICEBERG,
        AIVWAP,
        MSTD,
        UNKNOWN
    };

    enum class Status {
        READY,
        IN_PROGRESS,
        PAUSED,
        STOPPED,
        FINISHED,
        CAPITAL_INSUFFICIENT_ERROR,
        SECURITY_INSUFFICIENT_ERROR,
        UNKNOWN
    };

    class TaskInterface {

    public:
        TaskInterface() = default;

        virtual ~TaskInterface() = default;

    public:

        virtual Type get_type() const = 0;

        virtual void start(const nlohmann::json &params) = 0;

        virtual bool execution() = 0;

        virtual int32_t pause(const std::string &cli_session_id) = 0;

        virtual int32_t resume(const std::string &cli_session_id) = 0;

        virtual int32_t stop(const std::string &cli_session_id) = 0;

        virtual uint32_t get_task_id() const = 0;

        virtual int32_t stop() =0;

        virtual void set_calculator(std::shared_ptr<AssetPositionCalculator> calculator) = 0;

        virtual size_t get_order_uid() = 0;

        virtual size_t get_trade_uid() = 0;

        virtual void on_order_info(std::shared_ptr<OrderInfo> order_info, std::shared_ptr<ErrorInfo> error_info) = 0;

        virtual void on_trade_report(std::shared_ptr<TradeReport> trade_report) = 0;

        virtual bool set_xtp_order_id_market_data(ats::SideType side, double price, unsigned long xtp_order_id,
                                                  std::shared_ptr<ats::MarketQuotationData> marketquotationdata) = 0;

        virtual std::shared_ptr<ats::MarketData> get_current_market_data() const = 0;

        virtual std::pair<ats::PriceAction, std::shared_ptr<ats::MarketQuotationData>>
        find_market_quotation(uint64_t xtp_order_id) = 0;

        virtual void stop_and_set_msg(std::string remark_msg) = 0;

        virtual int  check_time_out(std::chrono::system_clock::time_point end_time) = 0;
    };
}

#endif //MUSE_TASKINTERFACE_H

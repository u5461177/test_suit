//
// Created by Rong Zhou on 2018-08-07.
//

#ifndef ATS_IQUOTESERVER_H
#define ATS_IQUOTESERVER_H

#include <memory>
#include "marketdata.h"
#include "orderbook.h"
#include "tickbytick.h"
#include "tickerprice.h"
#include <queue>

namespace ats {

    using Ticker = std::pair<std::string, MarketType>;

    struct IQuoteServer {

        virtual bool connect() = 0;

        virtual void disconnect() = 0;

        virtual void subscribe_ticker_price_info() = 0;

        virtual void subscribe_market_data(const Ticker &ticker) = 0;

        virtual void subscribe_order_book(const Ticker &ticker) = 0;

        virtual void subscribe_tick_by_tick(const Ticker &ticker) = 0;

        virtual void unsubscribe_market_data(const Ticker &ticker) = 0;

        virtual void unsubscribe_order_book(const Ticker &ticker) = 0;

        virtual void unsubscribe_tick_by_tick(const Ticker &ticker) = 0;

        virtual std::unique_ptr<MarketData> query_market_data(const Ticker &ticker) = 0;

        virtual std::shared_ptr<std::vector<std::shared_ptr<MarketData>>> query_market_data_queue(const Ticker &ticker) = 0;

        virtual std::unique_ptr<OrderBook> query_order_book(const Ticker &ticker) = 0;

        virtual std::unique_ptr<TickByTick> query_tick_by_tick(const Ticker &ticker) = 0;

        virtual std::unique_ptr<TickerPrice> query_tickers_price_info(const Ticker &ticker) = 0;

        virtual void on_disconnected() = 0;

        virtual void on_market_data(std::unique_ptr<MarketData> market_data) = 0;

        virtual void on_order_book(std::unique_ptr<OrderBook> order_book) = 0;

        virtual void on_tick_by_tick(std::unique_ptr<TickByTick> tick_by_tick) = 0;

        virtual void on_tickers_price_info(std::unique_ptr<TickerPrice> ticker_price) = 0;
    };

}

#endif //ATS_IQUOTESERVER_H

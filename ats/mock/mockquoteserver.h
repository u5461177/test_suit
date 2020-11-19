//
// Created by Rong Zhou on 2018-08-07.
//

#ifndef ATS_MOCKQUOTESERVER_H
#define ATS_MOCKQUOTESERVER_H

#include "singleton.h"
#include "iquoteserver.h"

namespace ats {

    class MockQuoteServer : public Singleton<MockQuoteServer>, IQuoteServer {
    public:
        MockQuoteServer() = default;

        ~MockQuoteServer() final = default;

    public:
        bool connect() override;

        void disconnect() override;

        void subscribe_market_data(const Ticker &ticker) override;

        void subscribe_order_book(const Ticker &ticker) override;

        void subscribe_tick_by_tick(const Ticker &ticker) override;

        void unsubscribe_market_data(const Ticker &ticker) override;

        void unsubscribe_order_book(const Ticker &ticker) override;

        void unsubscribe_tick_by_tick(const Ticker &ticker) override;

        std::unique_ptr<MarketData> query_market_data(const Ticker &ticker) override;

        std::unique_ptr<OrderBook> query_order_book(const Ticker &ticker) override;

        std::unique_ptr<TickByTick> query_tick_by_tick(const Ticker &ticker) override;

        void on_disconnected() override;

        void on_market_data(std::unique_ptr<MarketData> market_data) override;

        void on_order_book(std::unique_ptr<OrderBook> order_book) override;

        void on_tick_by_tick(std::unique_ptr<TickByTick> tick_by_tick) override;
    };
}

#endif //ATS_MOCKQUOTESERVER_H

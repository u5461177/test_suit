//
// Created by Rong Zhou on 2018-08-03.
//

#ifndef ATS_QUOTESERVER_H
#define ATS_QUOTESERVER_H

#include "singleton.h"
#include <map>
#include <shared_mutex>
#include "iquoteserver.h"
#include "quotespi.h"
#include <vector>

namespace ats {

    using Ticker = std::pair<std::string, MarketType>;

    class QuoteServer : public Singleton<QuoteServer>, IQuoteServer {
    public:
        QuoteServer();

        ~QuoteServer() final;

    public:
        bool connect() override;

        void disconnect() override;

        void subscribe_ticker_price_info() override;

        void subscribe_market_data(const Ticker &ticker) override;

        void subscribe_order_book(const Ticker &ticker) override;

        void subscribe_tick_by_tick(const Ticker &ticker) override;

        void unsubscribe_market_data(const Ticker &ticker) override;

        void unsubscribe_order_book(const Ticker &ticker) override;

        void unsubscribe_tick_by_tick(const Ticker &ticker) override;

        std::unique_ptr<MarketData> query_market_data(const Ticker &ticker) override;

        std::shared_ptr<std::vector<std::shared_ptr<MarketData>>> query_market_data_queue(const Ticker &ticker) override;

        std::unique_ptr<OrderBook> query_order_book(const Ticker &ticker) override;

        std::unique_ptr<TickByTick> query_tick_by_tick(const Ticker &ticker) override;

        std::unique_ptr<TickerPrice> query_tickers_price_info(const Ticker &ticker) override;

        void on_disconnected() override;

        void on_market_data(std::unique_ptr<MarketData> market_data) override;

        void on_order_book(std::unique_ptr<OrderBook> order_book) override;

        void on_tick_by_tick(std::unique_ptr<TickByTick> tick_by_tick) override;

        void on_tickers_price_info(std::unique_ptr<TickerPrice> ticker_price) override;

    private:
        XTP::API::QuoteApi *_quote_api;
        QuoteSPI _quote_spi;
        std::map<Ticker, std::pair<std::shared_ptr<MarketData>, size_t>> _market_data;
        std::map<Ticker, std::pair<std::shared_ptr<std::vector<std::shared_ptr<MarketData>>>, size_t>> _market_data_queue;
        std::map<Ticker, std::pair<std::unique_ptr<OrderBook>, size_t>> _order_book;
        std::map<Ticker, std::pair<std::unique_ptr<TickByTick>, size_t>> _tick_by_tick;
        std::map<Ticker, std::pair<std::unique_ptr<TickerPrice>, size_t>> _tickers_price_info;
        std::shared_mutex _market_data_mutex;
        std::shared_mutex _order_book_mutex;
        std::shared_mutex _tick_by_tick_mutex;
        std::shared_mutex _tickers_price_info_mutex;
    };

}

#endif //ATS_QUOTESERVER_H

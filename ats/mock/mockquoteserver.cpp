//
// Created by Rong Zhou on 2018-08-07.
//

#include "mockquoteserver.h"

namespace ats {

    using namespace std;

    bool MockQuoteServer::connect() {
        return true;
    }

    void MockQuoteServer::disconnect() {
    }

    void MockQuoteServer::subscribe_market_data(const Ticker &ticker) {
    }

    void MockQuoteServer::subscribe_order_book(const Ticker &ticker) {
    }

    void MockQuoteServer::subscribe_tick_by_tick(const Ticker &ticker) {
    }

    void MockQuoteServer::unsubscribe_market_data(const Ticker &ticker) {
    }

    void MockQuoteServer::unsubscribe_order_book(const Ticker &ticker) {
    }

    void MockQuoteServer::unsubscribe_tick_by_tick(const Ticker &ticker) {
    }

    unique_ptr<MarketData> MockQuoteServer::query_market_data(const Ticker &ticker) {
        return nullptr;
    }

    unique_ptr<OrderBook> MockQuoteServer::query_order_book(const Ticker &ticker) {
        return nullptr;
    }

    unique_ptr<TickByTick> MockQuoteServer::query_tick_by_tick(const Ticker &ticker) {
        return nullptr;
    }

    void MockQuoteServer::on_disconnected() {
    }

    void MockQuoteServer::on_market_data(unique_ptr<MarketData> market_data) {
    }

    void MockQuoteServer::on_order_book(unique_ptr<OrderBook> order_book) {
    }

    void MockQuoteServer::on_tick_by_tick(unique_ptr<TickByTick> tick_by_tick) {
    }

}
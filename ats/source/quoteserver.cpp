//
// Created by Rong Zhou on 2018-08-03.
//

#include "quoteserver.h"
#include "reposerver.h"
#include <thread>
#include "configuration.h"
#include "glog/logging.h"
#include "algorithm"
#include <assert.h>

namespace ats {

    using namespace std;

    QuoteServer::QuoteServer() {
        uint8_t client_id = Configuration::instance().get_quote_client_id();
        string file_path("/home/data/muse_log/data");
        _quote_api = XTP::API::QuoteApi::CreateQuoteApi(client_id, file_path.c_str());
    }

    QuoteServer::~QuoteServer() {
        disconnect();
        _quote_api = nullptr;
    }

    bool QuoteServer::connect() {
        auto server_info = Configuration::instance().get_quote_server_info();
        auto server_ip = get<0>(server_info);
        auto server_port = get<1>(server_info);
        auto username = get<2>(server_info);
        _quote_api->SetHeartBeatInterval(30);
        if (_quote_api->Login(server_ip.c_str(), server_port, username.c_str(),
                              Configuration::instance().get_quote_server_password().c_str(), XTP_PROTOCOL_TCP) == 0) {
            _quote_api->RegisterSpi(&_quote_spi);
            LOG(INFO) << "login quote server successfully";
            subscribe_ticker_price_info();
            return true;
        }
        //XTPRI *error = _quote_api->GetApiLastError();
        //LOG(ERROR) << "failed to login quote server: " << error->error_id << " | " << error->error_msg;
        return false;
    }

    void QuoteServer::disconnect() {
        if (_quote_api->Logout() == 0) {
            LOG(INFO) << "logout quote server successfully";
        } else {
            XTPRI *error = _quote_api->GetApiLastError();
            LOG(ERROR) << "failed to logout quote server: " << error->error_id << " | " << error->error_msg;
        }
    }

    void QuoteServer::subscribe_ticker_price_info() {
        _quote_api->QueryAllTickersPriceInfo();
    }

    void QuoteServer::subscribe_market_data(const Ticker &ticker) {
        lock_guard<shared_mutex> guard(_market_data_mutex);
        auto i = _market_data.find(ticker);
        if (i == _market_data.end()) {
            const char *ticker_id = ticker.first.c_str();
            _quote_api->SubscribeMarketData(const_cast<char **>(&ticker_id), 1,
                                            market_type_to_xtp_exchange_type(ticker.second));
            _market_data[ticker] = make_pair(nullptr, 1);
            _market_data_queue[ticker] = make_pair(nullptr, 1);
        } else {
            i->second.second++;
        }
    }

    void QuoteServer::subscribe_order_book(const Ticker &ticker) {
        lock_guard<shared_mutex> guard(_order_book_mutex);
        auto i = _order_book.find(ticker);
        if (i == _order_book.end()) {
            const char *ticker_id = ticker.first.c_str();
            _quote_api->SubscribeOrderBook(const_cast<char **>(&ticker_id), 1,
                                           market_type_to_xtp_exchange_type(ticker.second));
            _order_book[ticker] = make_pair(nullptr, 1);
        } else {
            i->second.second++;
        }
    }

    void QuoteServer::subscribe_tick_by_tick(const Ticker &ticker) {
        lock_guard<shared_mutex> guard(_tick_by_tick_mutex);
        auto i = _tick_by_tick.find(ticker);
        if (i == _tick_by_tick.end()) {
            const char *ticker_id = ticker.first.c_str();
            _quote_api->SubscribeTickByTick(const_cast<char **>(&ticker_id), 1,
                                            market_type_to_xtp_exchange_type(ticker.second));
            _tick_by_tick[ticker] = make_pair(nullptr, 1);
        } else {
            i->second.second++;
        }
    }

    void QuoteServer::unsubscribe_market_data(const Ticker &ticker) {
        lock_guard<shared_mutex> guard(_market_data_mutex);
        auto i = _market_data.find(ticker);
        auto j = _market_data_queue.find(ticker);
        if (i not_eq _market_data.end() && j not_eq _market_data_queue.end()) {
            if (0 == --_market_data[ticker].second && 0 == --_market_data_queue[ticker].second) {
                const char *ticker_id = ticker.first.c_str();
                _quote_api->UnSubscribeMarketData(const_cast<char **>(&ticker_id), 1,
                                                  market_type_to_xtp_exchange_type(ticker.second));
                _market_data.erase(i);
                _market_data_queue.erase(j);
            }
        }
    }

    void QuoteServer::unsubscribe_order_book(const Ticker &ticker) {
        lock_guard<shared_mutex> guard(_order_book_mutex);
        auto i = _order_book.find(ticker);
        if (i not_eq _order_book.end()) {
            if (0 == --_order_book[ticker].second) {
                const char *ticker_id = ticker.first.c_str();
                _quote_api->UnSubscribeOrderBook(const_cast<char **>(&ticker_id), 1,
                                                 market_type_to_xtp_exchange_type(ticker.second));
                _order_book.erase(i);
            }
        }
    }

    void QuoteServer::unsubscribe_tick_by_tick(const Ticker &ticker) {
        lock_guard<shared_mutex> guard(_tick_by_tick_mutex);
        auto i = _tick_by_tick.find(ticker);
        if (i not_eq _tick_by_tick.end()) {
            if (0 == --_tick_by_tick[ticker].second) {
                const char *ticker_id = ticker.first.c_str();
                _quote_api->UnSubscribeTickByTick(const_cast<char **>(&ticker_id), 1,
                                                  market_type_to_xtp_exchange_type(ticker.second));
                _tick_by_tick.erase(i);
            }
        }
    }

    unique_ptr<MarketData> QuoteServer::query_market_data(const Ticker &ticker) {
        shared_lock<shared_mutex> lock(_market_data_mutex);
        auto i = _market_data.find(ticker);
        if (i not_eq _market_data.end()) {
            const auto &market_data = i->second.first;
            if (market_data) {
                LOG(INFO) << "market data update | ticker: " << ticker.first;
                return make_unique<MarketData>(*market_data);
            }
        }
        return nullptr;
    }

    shared_ptr<vector<shared_ptr<MarketData>>> QuoteServer::query_market_data_queue(const Ticker &ticker) {
        shared_lock<shared_mutex> lock(_market_data_mutex);
        auto i = _market_data_queue.find(ticker);
        if (i not_eq _market_data_queue.end()) {
            const auto &market_data_queue = i->second.first;
            if (market_data_queue) {
                shared_ptr<std::vector<shared_ptr<MarketData>>> orderbook = make_shared<vector<shared_ptr<MarketData>>>();
                for(auto const& market_data: *market_data_queue) {
                    orderbook->push_back(std::make_shared<MarketData>(*market_data));
                }
                return orderbook;
            }
        }
        return nullptr;
    }

    unique_ptr<OrderBook> QuoteServer::query_order_book(const Ticker &ticker) {
        shared_lock<shared_mutex> lock(_order_book_mutex);
        auto i = _order_book.find(ticker);
        if (i not_eq _order_book.end()) {
            const auto &order_book = i->second.first;
            if (order_book) {
                return make_unique<OrderBook>(*order_book);
            }
        }
        return nullptr;
    }

    unique_ptr<TickByTick> QuoteServer::query_tick_by_tick(const Ticker &ticker) {
        shared_lock<shared_mutex> lock(_tick_by_tick_mutex);
        auto i = _tick_by_tick.find(ticker);
        if (i not_eq _tick_by_tick.end()) {
            const auto &tick_by_tick = i->second.first;
            if (tick_by_tick) {
                return make_unique<TickByTick>(*tick_by_tick);
            }
        }
        return nullptr;
    }

    unique_ptr<TickerPrice> QuoteServer::query_tickers_price_info(const Ticker &ticker) {
        shared_lock<shared_mutex> lock(_tickers_price_info_mutex);
        auto i = _tickers_price_info.find(ticker);
        if (i not_eq _tickers_price_info.end()) {
            const auto &tickers_price_info = i->second.first;
            if (tickers_price_info) {
                return make_unique<TickerPrice>(*tickers_price_info);
            }
        }
        return nullptr;
    }

    void QuoteServer::on_disconnected() {
        while (!connect()) {
            this_thread::sleep_for(5s);
        }
        for (const auto &market_data : _market_data) {
            auto ticker = market_data.first;
            const char *ticker_id = ticker.first.c_str();
            _quote_api->SubscribeMarketData(const_cast<char **>(&ticker_id), 1,
                                            market_type_to_xtp_exchange_type(ticker.second));
        }
        for (const auto &orderbook : _order_book) {
            auto ticker = orderbook.first;
            const char *ticker_id = ticker.first.c_str();
            _quote_api->SubscribeOrderBook(const_cast<char **>(&ticker_id), 1,
                                           market_type_to_xtp_exchange_type(ticker.second));
        }
        for (const auto &tick_by_tick : _tick_by_tick) {
            auto ticker = tick_by_tick.first;
            const char *ticker_id = ticker.first.c_str();
            _quote_api->SubscribeTickByTick(const_cast<char **>(&ticker_id), 1,
                                            market_type_to_xtp_exchange_type(ticker.second));
        }
    }

    void QuoteServer::on_market_data(unique_ptr<MarketData> market_data) {
        lock_guard<shared_mutex> guard(_market_data_mutex);
        shared_ptr<MarketData> md = move(market_data);
        auto ticker = make_pair(md->ticker, md->market);
        _market_data[ticker].first = md;

        if (find(repo_ticker.begin(), repo_ticker.end(), ticker) != repo_ticker.end())
        	RepoServer::instance().update_repo_data(_market_data[ticker].first->ticker, _market_data[ticker].first->ask[0]);

        if(_market_data_queue[ticker].first==nullptr){
            _market_data_queue[ticker].first = make_shared<vector<shared_ptr<MarketData>>>();
        }
        _market_data_queue[ticker].first->push_back(md);
        if (_market_data_queue[ticker].first->size() >= 26) {
            //pop front
            unsigned long i = _market_data_queue[ticker].first->size() - 25;
            _market_data_queue[ticker].first->erase(_market_data_queue[ticker].first->begin(),
                                                    _market_data_queue[ticker].first->begin() + i);
        }
        while (_market_data_queue[ticker].first->size() < 25) {
            _market_data_queue[ticker].first->push_back(md);
        }
        assert(_market_data_queue[ticker].first->size() == 25);
    }

    void QuoteServer::on_order_book(unique_ptr<OrderBook> order_book) {
        lock_guard<shared_mutex> guard(_order_book_mutex);
        auto ticker = make_pair(order_book->ticker, order_book->market);
        _order_book[ticker].first = move(order_book);
    }

    void QuoteServer::on_tick_by_tick(unique_ptr<TickByTick> tick_by_tick) {
        lock_guard<shared_mutex> guard(_tick_by_tick_mutex);
        auto ticker = make_pair(tick_by_tick->ticker, tick_by_tick->market);
        _tick_by_tick[ticker].first = move(tick_by_tick);
    }

    void QuoteServer::on_tickers_price_info(unique_ptr<TickerPrice> ticker_price) {
        lock_guard<shared_mutex> guard(_tickers_price_info_mutex);
        auto ticker = make_pair(ticker_price->ticker, ticker_price->market);
        _tickers_price_info[ticker].first = move(ticker_price);
    }
}

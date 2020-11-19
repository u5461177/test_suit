//
// Created by Rong Zhou on 2018-08-03.
//

#include "quotespi.h"
#include <cassert>
#include "tickerprice.h"
#include "redisserver.h"
#include "quoteserver.h"
#include "glog/logging.h"

namespace ats {

    using namespace std;

    void QuoteSPI::OnDisconnected(int reason) {
        LOG(WARNING) << "quote server disconnected: " << reason;
        QuoteServer::instance().on_disconnected();
    }

    void QuoteSPI::OnError(XTPRI *error) {
        LOG(ERROR) << "quote server error: " << error->error_id << " | " << error->error_msg;
    }

    void QuoteSPI::OnSubMarketData(XTPST *ticker, XTPRI *error, bool finished) {
        if (error->error_id > 0) {
            LOG(ERROR) << "subscribe market data error: " << error->error_id << " | " << error->error_msg;
        } else {
            //LOG(INFO) << "log subscribe market data: " << ticker->ticker;
        }
        assert(finished);
    }

    void QuoteSPI::OnSubOrderBook(XTPST *ticker, XTPRI *error, bool finished) {
        if (error->error_id > 0) {
            LOG(ERROR) << "subscribe order book error: " << error->error_id << " | " << error->error_msg;
        } else {
            LOG(INFO) << "log subscribe order book: " << ticker->ticker;
        }
        assert(finished);
    }

    void QuoteSPI::OnSubTickByTick(XTPST *ticker, XTPRI *error, bool finished) {
        if (error->error_id > 0) {
            LOG(ERROR) << "subscribe tick by tick error: " << error->error_id << " | " << error->error_msg;
        } else {
            LOG(INFO) << "log subscribe tick by tick: " << ticker->ticker;
        }
        assert(finished);
    }

    void QuoteSPI::OnUnSubMarketData(XTPST *ticker, XTPRI *error, bool finished) {
        if (error->error_id > 0) {
            LOG(ERROR) << "unsubscribe market data error: " << error->error_id << " | " << error->error_msg;
        } else {
            LOG(INFO) << "log unsubscribe market data: " << ticker->ticker;
        }
        assert(finished);
    }

    void QuoteSPI::OnUnSubOrderBook(XTPST *ticker, XTPRI *error, bool finished) {
        if (error->error_id > 0) {
            LOG(ERROR) << "unsubscribe order book error: " << error->error_id << " | " << error->error_msg;
        } else {
            LOG(INFO) << "log unsubscribe order book: " << ticker->ticker;
        }
        assert(finished);
    }

    void QuoteSPI::OnUnSubTickByTick(XTPST *ticker, XTPRI *error, bool finished) {
        if (error->error_id > 0) {
            LOG(ERROR) << "unsubscribe tick by tick error: " << error->error_id << " | " << error->error_msg;
        } else {
            LOG(INFO) << "log unsubscribe tick by tick: " << ticker->ticker;
        }
        assert(finished);
    }

    void QuoteSPI::OnDepthMarketData(XTPMD *market_data, int64_t bid1_qty[], int32_t bid1_count, int32_t max_bid1_count,
                                     int64_t ask1_qty[], int32_t ask1_count, int32_t max_ask1_count) {
//        LOG_EVERY_N(INFO, 20) << "on depth market data";
        QuoteServer::instance().on_market_data(make_unique<MarketData>(market_data));
    }

    void QuoteSPI::OnOrderBook(XTPOB *order_book) {
        LOG(INFO) << "on order book";
        QuoteServer::instance().on_order_book(make_unique<OrderBook>(order_book));
    }

    void QuoteSPI::OnTickByTick(XTPTBT *tick_by_tick) {
        LOG(INFO) << "on tick by tick";
        QuoteServer::instance().on_tick_by_tick(make_unique<TickByTick>(tick_by_tick));
    }

    void QuoteSPI::OnQueryTickersPriceInfo(XTPTPI *ticker_price, XTPRI *error, bool finished) {
        if (error->error_id > 0) {
            LOG(ERROR) << "query tickers price info error: " << error->error_id << " | " << error->error_msg;
            this_thread::sleep_for(1s);
            QuoteServer::instance().subscribe_ticker_price_info();
        } else {
            QuoteServer::instance().on_tickers_price_info(make_unique<TickerPrice>(ticker_price));
        }
    }

}
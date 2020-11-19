//
// Created by Rong Zhou on 2018-08-07.
//

#include "mockquotespi.h"

namespace ats {

    void MockQuoteSPI::OnDisconnected(int reason) {

    }

    void MockQuoteSPI::OnError(XTPRI *error) {

    }

    void MockQuoteSPI::OnSubMarketData(XTPST *ticker, XTPRI *error, bool finished) {

    }

    void MockQuoteSPI::OnSubOrderBook(XTPST *ticker, XTPRI *error, bool finished) {

    }

    void MockQuoteSPI::OnSubTickByTick(XTPST *ticker, XTPRI *error, bool finished) {

    }

    void MockQuoteSPI::OnUnSubMarketData(XTPST *ticker, XTPRI *error, bool finished) {

    }

    void MockQuoteSPI::OnUnSubOrderBook(XTPST *ticker, XTPRI *error, bool finished) {

    }

    void MockQuoteSPI::OnUnSubTickByTick(XTPST *ticker, XTPRI *error, bool finished) {

    }

    void
    MockQuoteSPI::OnDepthMarketData(XTPMD *market_data, int64_t bid1_qty[], int32_t bid1_count, int32_t max_bid1_count,
                                    int64_t ask1_qty[], int32_t ask1_count, int32_t max_ask1_count) {

    }

    void MockQuoteSPI::OnOrderBook(XTPOB *order_book) {

    }

    void MockQuoteSPI::OnTickByTick(XTPTBT *tick_by_tick) {

    }

    void MockQuoteSPI::OnQueryTickersPriceInfo(XTPTPI *ticker_price, XTPRI *error, bool finished) {

    }

}
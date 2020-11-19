//
// Created by Rong Zhou on 2018-08-03.
//

#ifndef ATS_QUOTESPI_H
#define ATS_QUOTESPI_H

#include <xtp_quote_api.h>

namespace ats {

    struct QuoteSPI : public XTP::API::QuoteSpi {

        void OnDisconnected(int reason) override;

        void OnError(XTPRI *error) override;

        void OnSubMarketData(XTPST *ticker, XTPRI *error, bool finished) override;

        void OnSubOrderBook(XTPST *ticker, XTPRI *error, bool finished) override;

        void OnSubTickByTick(XTPST *ticker, XTPRI *error, bool finished) override;

        void OnUnSubMarketData(XTPST *ticker, XTPRI *error, bool finished) override;

        void OnUnSubOrderBook(XTPST *ticker, XTPRI *error, bool finished) override;

        void OnUnSubTickByTick(XTPST *ticker, XTPRI *error, bool finished) override;

        void OnDepthMarketData(XTPMD *market_data, int64_t bid1_qty[], int32_t bid1_count, int32_t max_bid1_count,
                               int64_t ask1_qty[], int32_t ask1_count, int32_t max_ask1_count) override;

        void OnOrderBook(XTPOB *order_book) override;

        void OnTickByTick(XTPTBT *tick_by_tick) override;

        void OnQueryTickersPriceInfo(XTPTPI *ticker_price, XTPRI *error, bool finished) override;
    };

}

#endif //ATS_QUOTESPI_H

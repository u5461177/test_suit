//
// Created by Rong Zhou on 2018-08-07.
//

#ifndef ATS_MOCKTRADERSPI_H
#define ATS_MOCKTRADERSPI_H

#include <xtp_trader_api.h>

namespace ats {

    struct MockTraderSPI : public XTP::API::TraderSpi {

        void OnDisconnected(uint64_t xtp_session_id, int reason) override;

        void OnError(XTPRI *error) override;

        void OnOrderEvent(XTPOrderInfo *order_info, XTPRI *error, uint64_t xtp_session_id) override;

        void OnTradeEvent(XTPTradeReport *trade_report, uint64_t xtp_session_id) override;

        void OnCancelOrderError(XTPOrderCancelInfo *cancel_order_info, XTPRI *error, uint64_t xtp_session_id) override;

        void OnQueryOrder(XTPQueryOrderRsp *order_info, XTPRI *error, int request_id, bool finished,
                          uint64_t xtp_session_id) override;

        void OnQueryTrade(XTPQueryTradeRsp *trade_report, XTPRI *error, int request_id, bool finished,
                          uint64_t xtp_session_id) override;

        void OnQueryAsset(XTPQueryAssetRsp *asset, XTPRI *error, int request_id, bool finished,
                          uint64_t xtp_session_id) override;

        void OnQueryPosition(XTPQueryStkPositionRsp *stk_position, XTPRI *error, int request_id, bool finished,
                             uint64_t xtp_session_id) override;
    };

};

#endif //ATS_MOCKTRADERSPI_H

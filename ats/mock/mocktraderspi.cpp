//
// Created by Rong Zhou on 2018-08-07.
//

#include "mocktraderspi.h"

namespace ats {

    void MockTraderSPI::OnDisconnected(uint64_t xtp_session_id, int reason) {

    }

    void MockTraderSPI::OnError(XTPRI *error) {

    }

    void MockTraderSPI::OnOrderEvent(XTPOrderInfo *order_info, XTPRI *error, uint64_t xtp_session_id) {

    }

    void MockTraderSPI::OnTradeEvent(XTPTradeReport *trade_report, uint64_t xtp_session_id) {

    }

    void
    MockTraderSPI::OnCancelOrderError(XTPOrderCancelInfo *cancel_order_info, XTPRI *error, uint64_t xtp_session_id) {

    }

    void MockTraderSPI::OnQueryOrder(XTPQueryOrderRsp *order_info, XTPRI *error, int request_id, bool finished,
                                     uint64_t xtp_session_id) {

    }

    void MockTraderSPI::OnQueryTrade(XTPQueryTradeRsp *trade_report, XTPRI *error, int request_id, bool finished,
                                     uint64_t xtp_session_id) {

    }

    void MockTraderSPI::OnQueryAsset(XTPQueryAssetRsp *asset, XTPRI *error, int request_id, bool finished,
                                     uint64_t xtp_session_id) {

    }

    void
    MockTraderSPI::OnQueryPosition(XTPQueryStkPositionRsp *stk_position, XTPRI *error, int request_id, bool finished,
                                   uint64_t xtp_session_id) {

    }

}
//
// Created by Rong Zhou on 2018-08-03.
//

#ifndef ATS_TRADERSPI_H
#define ATS_TRADERSPI_H

#include <string>
#include <xtp_trader_api.h>
#include "assetpositioncalculator.h"
#include "user.h"

namespace ats {

    struct TraderSPI : public XTP::API::TraderSpi {

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

}

#endif //ATS_TRADERSPI_H

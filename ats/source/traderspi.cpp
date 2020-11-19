//
// Created by rongzhou on 18-7-29.
//

#include "traderspi.h"
#include "redisserver.h"
#include "usermanager.h"
#include "ordercancelinfo.h"

namespace ats {

    using namespace std;

    void TraderSPI::OnDisconnected(uint64_t xtp_session_id, int reason) {
        LOG(WARNING) << "trader server disconnected for session id: " << xtp_session_id << " | " << reason;
        UserManager::instance().on_disconnected(xtp_session_id);
    }

    void TraderSPI::OnError(XTPRI *error) {
        LOG(ERROR) << "trader server error: " << error->error_id << " | " << error->error_msg;
    }

    void TraderSPI::OnOrderEvent(XTPOrderInfo *order_info, XTPRI *error, uint64_t xtp_session_id) {
        LOG(WARNING) << "on order event";
        if (error->error_id != 0) {
            LOG(WARNING) << "order event error: " << error->error_id << " | " << error->error_msg;
        }
        if (auto user = UserManager::instance().find_user(xtp_session_id)) {
            LOG(INFO) << "on order info send to user";
            user->on_order_info(make_shared<OrderInfo>(order_info), make_shared<ErrorInfo>(error));
        }
    }

    void TraderSPI::OnTradeEvent(XTPTradeReport *trade_report, uint64_t xtp_session_id) {
        LOG(WARNING) << "on trade event";
        if (auto user = UserManager::instance().find_user(xtp_session_id)) {
            user->on_trade_report(make_shared<TradeReport>(trade_report));
        }
    }

    void TraderSPI::OnCancelOrderError(XTPOrderCancelInfo *cancel_order_info, XTPRI *error, uint64_t xtp_session_id) {
        LOG(ERROR) << "cancel order error event | order xtp id: " << cancel_order_info->order_xtp_id
                   << " order cancel xtp id: " << cancel_order_info->order_cancel_xtp_id;
        if (error->error_id != 0) {
            LOG(ERROR) << "order cancel event error: " << error->error_id << " | " << error->error_msg;
        }
        if(auto user = UserManager::instance().find_user(xtp_session_id)) {
            LOG(INFO) << "on cancel order error";
            user->on_cancel_order_info(make_shared<OrderCancelInfo>(cancel_order_info), make_shared<ErrorInfo>(error));
        }
    }

    void TraderSPI::OnQueryOrder(XTPQueryOrderRsp *order_info, XTPRI *error, int /*request_id*/, bool finished,
                                 uint64_t xtp_session_id) {
        if (error->error_id != 0) {
            LOG(WARNING) << "query order error: " << error->error_id << " | " << error->error_msg;
        }
        if (auto user = UserManager::instance().find_user(xtp_session_id)) {
            user->on_order_info(make_shared<OrderInfo>(order_info), make_shared<ErrorInfo>(error));
        }

        assert(finished);
    }

    void TraderSPI::OnQueryTrade(XTPQueryTradeRsp *trade_report, XTPRI *error, int /*request_id*/, bool finished,
                                 uint64_t xtp_session_id) {
        if (error->error_id == 0) {
            if (auto user = UserManager::instance().find_user(xtp_session_id)) {
                user->on_trade_report(make_shared<TradeReport>(trade_report));
            }
        } else {
            LOG(WARNING) << "query trade error: " << error->error_id << " | " << error->error_msg;
        }
        assert(finished);
    }

    void TraderSPI::OnQueryAsset(XTPQueryAssetRsp *asset, XTPRI *error, int /*request_id*/, bool finished,
                                 uint64_t xtp_session_id) {
        LOG(WARNING) << "on query asset";
        static const string user_asset("user_asset");
        if (error->error_id == 0) {
            if (auto user = UserManager::instance().find_user(xtp_session_id)) {
                user->on_asset(make_shared<UserAsset>(asset), true);
                LOG(INFO) << "xtp current buying power : " << asset->buying_power;
            }
        } else {
            LOG(WARNING) << "query asset error: " << error->error_id << " | " << error->error_msg;
        }
        assert(finished);
    }

    void
    TraderSPI::OnQueryPosition(XTPQueryStkPositionRsp *stk_position, XTPRI *error, int request_id, bool finished,
                               uint64_t xtp_session_id) {
        LOG(WARNING) << "on query position";
        if (error->error_id == 0) {
            if (auto user = UserManager::instance().find_user(xtp_session_id)) {
                user->on_stock_position(stk_position->ticker, make_shared<StockPosition>(stk_position), true);
                LOG(INFO) << "xtp current sellable qty : " << stk_position->sellable_qty;
            }
        } else if (error->error_id == 11000350) {
            string ticker = to_string(request_id);
            string pre_str(6 - ticker.size(),'0');
            ticker = pre_str + ticker;
            if (auto user = UserManager::instance().find_user(xtp_session_id)) {
                user->on_stock_position(ticker, make_shared<StockPosition>(ticker), true);
            }
        } else {
            LOG(WARNING) << "query position error: " << error->error_id << " | " << error->error_msg;
        }
        assert(finished);
    }

}
//
// Created by Rong Zhou on 2018-08-07.
//

#include "mocktraderserver.h"

namespace ats {

    using namespace std;

    pair<error_code, uint64_t>
    MockTraderServer::connect(const string &username, const string &password, const string &server_ip,
                              int server_port) {
        return make_pair(ErrorCode::NO_ERROR, 0);
    }

    error_code MockTraderServer::disconnect(uint64_t xtp_session_id) {
        return ErrorCode::NO_ERROR;
    }


    uint64_t MockTraderServer::place_order(uint32_t task_id, uint64_t xtp_session_id, const ats::Ticker &ticker,
                                           const ats::Price &price, const ats::Quantity &quantity,
                                           const ats::BusinessType &businessType) {
        return 0;
    }

    void MockTraderServer::cancel_order(uint64_t xtp_session_id, uint64_t order_id) {
    }

    void MockTraderServer::query_user_asset(uint64_t xtp_session_id) {
    }

    void MockTraderServer::query_stock_position(uint64_t xtp_session_id, const std::string &ticker_id) {
    }

    void MockTraderServer::query_order_info(uint64_t xtp_session_id, uint64_t order_id) {
    }

    void MockTraderServer::query_trade_info(uint64_t xtp_session_id, uint64_t order_id) {
    }

}
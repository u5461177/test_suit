//
// Created by Rong Zhou on 2018-08-03.
//

#include <iostream>
#include "traderserver.h"
#include "configuration.h"
#include "glog/logging.h"

namespace ats {

    using namespace std;

    TraderServer::TraderServer() {
        // TODO: For open test
        uint8_t client_id = Configuration::instance().get_trade_client_id();
        string file_path("/home/data/muse_log/data");
        _trader_api = XTP::API::TraderApi::CreateTraderApi(client_id, file_path.c_str());
        _trader_api->SubscribePublicTopic(XTP_TERT_QUICK);
        auto key = Configuration::instance().get_trader_server_key();
        _trader_api->SetSoftwareKey(key.c_str());
        LOG(INFO) << "KEY:" << key;
        _trader_api->SetSoftwareVersion("1.1");
        _trader_api->RegisterSpi(&_trader_spi);
    }

    pair<ErrorCategory, uint64_t>
    TraderServer::connect(const string &username, const string &password, const string &server_ip, int server_port) {
        LOG(INFO) << username + " about to login: " + server_ip + ":" + std::to_string(server_port);
        auto xtp_session_id = _trader_api->Login(server_ip.c_str(), server_port, username.c_str(),
                                                 password.c_str(), XTP_PROTOCOL_TCP);
        if (xtp_session_id > 0) {
            LOG(INFO) << "login trader server successfully";
            return make_pair(ErrorCategory(ErrorCode::NO_ERROR), xtp_session_id);
        }
        XTPRI *error = _trader_api->GetApiLastError();
        string error_msg = error->error_msg;
        LOG(ERROR) << error_msg;
        if (error_msg.find("user") not_eq string::npos) {
            return make_pair(ErrorCategory(ErrorCode::USER_NOT_EXIST), 0);
        }
        if (error_msg.find("password") not_eq string::npos) {
            return make_pair(ErrorCategory(ErrorCode::WRONG_PASSWORD), 0);
        }
        if (error_msg.find("connect") not_eq string::npos) {
            return make_pair(ErrorCategory(ErrorCode::WRONG_SERVER_IP), 0);
        }
        return make_pair(ErrorCategory(ErrorCode::UNKNOWN_ERROR), 0);
    }

    ErrorCategory TraderServer::disconnect(uint64_t xtp_session_id) {
        if (_trader_api->Logout(xtp_session_id) == 0) {
            LOG(INFO) << "logout trader server successfully";
            return ErrorCategory(ErrorCode::NO_ERROR);
        }
        XTPRI *error = _trader_api->GetApiLastError();
        LOG(ERROR) << "fail to logout trader server: " << error->error_id << " | " << error->error_msg;
        return ErrorCategory(ErrorCode::UNKNOWN_ERROR);
    }

    uint64_t
    TraderServer::place_order(uint32_t task_id, uint64_t xtp_session_id, const Ticker &ticker, const Price &price,
                              const Quantity &quantity, const BusinessType &business_type) {
        LOG(INFO) << "place order: " << price.first << " | " << quantity.first <<
                  " | task id: " << task_id;

        XTPOrderInsertInfo order_info{};
        uint64_t xtp_order_id = 0;
        order_info.order_xtp_id = 0;
        order_info.order_client_id = task_id;
        ticker.first.copy(order_info.ticker, ticker.first.size());
        order_info.ticker[ticker.first.size()] = '\0';
        order_info.market = market_type_to_xtp_market_type(ticker.second);
        order_info.price = price.first;
        order_info.price_type = price_type_to_xtp_price_type(price.second);
        order_info.quantity = quantity.first;
        order_info.side = side_type_to_xtp_side_type(quantity.second);
        order_info.business_type = business_type_to_xtp_business_type(business_type);
        xtp_order_id = _trader_api->InsertOrder(&order_info, xtp_session_id);
        return xtp_order_id;
    }

}

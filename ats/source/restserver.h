//
// Created by rongzhou on 18-7-29.
//

#ifndef ATS_RESTSERVER_H
#define ATS_RESTSERVER_H

#include "singleton.h"
#include "types/errorcode.h"
#include "quoteserver.h"
#include "types/markettype.h"
#include "types/sidetype.h"
#include "assetpositioncalculator.h"
#include "util/common.h"
#include "types/sidetype.h"
#include <json.hpp>
#include <restbed>
#include <map>
#include <shared_mutex>
#include <list>
#include <exception>

namespace ats {

    class RESTServer : public Singleton<RESTServer> {
    public:
        RESTServer() = default;

        ~RESTServer() final;

    public:
        void start();

        void stop();

    private:
        bool get_user_permission(const std::string &username);

        void on_user_login(std::shared_ptr<restbed::Session> session);

        void on_user_logout(std::shared_ptr<restbed::Session> session);

        void on_task_start(std::shared_ptr<restbed::Session> session);

        void on_tasks_start(std::shared_ptr<restbed::Session> session);

        void on_task_control(std::shared_ptr<restbed::Session> session);

        void on_task_report(std::shared_ptr<restbed::Session> session);

        void on_minute_report(std::shared_ptr<restbed::Session> session);

        void on_websocket_connected(std::shared_ptr<restbed::Session> session);

        void on_filter_validation_failed(std::shared_ptr<restbed::Session> session);

        void on_resource_error_handler(const int ,const std::exception& e, const std::shared_ptr<restbed::Session> session);

        void on_service_error_handler(const int ,const std::exception& e,const std::shared_ptr<restbed::Session> session);

        void on_schedule();

        void on_rest_response(const nlohmann::json &data, std::shared_ptr<restbed::Session> session);

        void on_rest_response(const std::string &data, std::shared_ptr<restbed::Session> session);

        void print(const std::shared_ptr<restbed::Response> &response);

        std::string vector_to_string(const std::vector<std::string> &report);

        std::string list_to_string(const std::list<std::string> &report);

        bool check_time_string(std::string &time_str);

    private:
        restbed::Service _rest_service;
        std::pair<std::string, int> _permission_server_info;
        std::vector<std::shared_ptr<restbed::WebSocket>> _websockets;
        std::shared_mutex _shared_mutex;
        std::map<std::string, std::vector<uint64_t>> _user_ids;
        std::shared_mutex _tasks_report_mutex;
        int _max_tasks_size;

    private:
        nlohmann::json basic_task_parser(nlohmann::json param, std::string port_name);

        double get_market_data(std::string &ticker_id, ats::MarketType &market);

        bool check_parameters(const std::string &input_parameter, nlohmann::json &param, int min_qty, int max_qty,
                              double price_base, bool &amount_flag, const BusinessType &businessType);

        std::shared_ptr<ats::AssetPositionCalculator> _create_calculator(BusinessType &businessType,
                                                                         ats::MarketType &marketType,
                                                                         ats::SideType &sideType);

        void _tasks_unit(nlohmann::json param, std::shared_ptr<std::list<std::string>> tasks_response,
                         std::string port_name);

    };

}

#endif //ATS_RESTSERVER_H

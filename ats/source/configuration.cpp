//
// Created by Rong Zhou on 2018-08-03.
//

#include "configuration.h"
#include <fstream>
#include <json.hpp>

using namespace std;
using json = nlohmann::json;

namespace ats {

    bool Configuration::load(const string &filename) {
        ifstream ifs(filename);
        try {
            json config = json::parse(ifs);
            auto redis_server_ip = config["redis_server_ip"].get<string>();
            auto redis_server_port = config["redis_server_port"].get<int>();
            _redis_server_info = make_pair(redis_server_ip, redis_server_port);

            auto rest_server_ip = config["rest_server_ip"].get<string>();
            auto rest_server_port = config["rest_server_port"].get<uint16_t>();
            _rest_server_info = make_pair(rest_server_ip, rest_server_port);

            auto quote_server_ip = config["quote_server_ip"].get<string>();
            auto quote_server_port = config["quote_server_port"].get<int>();
            auto quote_server_username = config["quote_server_username"].get<string>();
            _quote_server_info = make_tuple(quote_server_ip, quote_server_port, quote_server_username);

            auto elastic_search_ip = config["elastic_search_ip"].get<string>();
            auto elastic_search_port = config["elastic_search_port"].get<uint16_t>();
            _elastic_search_info = make_pair(elastic_search_ip, elastic_search_port);

            auto permission_server_ip = config["permission_server_ip"].get<string>();
            auto permission_server_port = config["permission_server_port"].get<int>();
            _permission_server_info = make_pair(permission_server_ip, permission_server_port);

            auto trade_client_id = config["trade_client_id"].get<uint8_t>();
            _trade_client_id = trade_client_id;

            auto quote_client_id = config["quote_client_id"].get<uint8_t>();
            _quote_client_id = quote_client_id;

            auto trader_server_key = config["trader_server_key"].get<string>();
            _trader_server_key = trader_server_key;

            auto quote_server_password = config["quote_server_password"].get<string>();
            _quote_server_password = quote_server_password;

            auto trade_time_limit = config["trade_time_limit"].get<bool>();
            _trade_time_limit = trade_time_limit;

            auto aivwap_model = config["aivwap_model"].get<string>();
            _aivwap_model = aivwap_model;

            auto mysql_server_ip = config["mysql_server_ip"].get<string>();
            auto mysql_server_port = config["mysql_server_port"].get<int>();
            _mysql_server_info = make_pair(mysql_server_ip, mysql_server_port);

            auto mysql_server_username = config["mysql_server_username"].get<string>();
            auto mysql_server_db_name = config["mysql_server_db_name"].get<string>();
            auto mysql_server_password = config["mysql_server_password"].get<string>();
            _mysql_db_info = make_tuple(mysql_server_username, mysql_server_password, mysql_server_db_name);

            _current_store = config["current_store"].get<bool>();

            _tasks_limit_num = config["tasks_limit_num"].get<int>();


        } catch (const json::exception &e) {
            return false;
        }
        return true;
    }

}
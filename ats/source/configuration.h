//
// Created by Rong Zhou on 2018-08-03.
//

#ifndef ATS_CONFIGURATION_H
#define ATS_CONFIGURATION_H

#include "singleton.h"
#include <string>
#include <tuple>

namespace ats {

    class Configuration : public Singleton<Configuration> {
    public:
        Configuration() = default;

        ~Configuration() final = default;

    public:
        bool load(const std::string &filename);

        inline std::pair<std::string, int> get_redis_server_info() const;

        inline std::pair<std::string, uint16_t> get_rest_server_info() const;

        inline std::tuple<std::string, int, std::string> get_quote_server_info() const;

        inline std::pair<std::string, int> get_elastic_search_info() const;

        inline std::pair<std::string, int> get_permission_server_info() const;

        inline uint8_t get_trade_client_id() const;

        inline uint8_t get_quote_client_id() const;

        inline std::string get_trader_server_key() const;

        inline std::string get_quote_server_password() const;

        inline bool get_trade_time_limit() const;

        inline std::string get_aivwap_model() const;

        inline std::pair<std::string, int> get_mysql_server_info() const;

        inline std::tuple<std::string, std::string, std::string> get_mysql_db_info() const;

        inline bool get_current_store() const;

        inline int get_tasks_limit_num() const;

    private:
        std::pair<std::string, int> _redis_server_info;
        std::pair<std::string, uint16_t> _rest_server_info;
        std::tuple<std::string, int, std::string> _quote_server_info;
        std::pair<std::string, int> _elastic_search_info;
        std::pair<std::string, int> _permission_server_info;
        uint8_t _trade_client_id = 0;
        uint8_t _quote_client_id = 0;
        std::string _trader_server_key;
        std::string _quote_server_password;
        bool _trade_time_limit;
        std::string _aivwap_model;
        std::pair<std::string, int> _mysql_server_info;
        std::tuple<std::string, std::string ,std::string> _mysql_db_info;
        bool _current_store;
        int _tasks_limit_num;
    };

    inline int  Configuration::get_tasks_limit_num() const {
        return _tasks_limit_num;
    }

    inline std::pair<std::string, int> Configuration::get_redis_server_info() const {
        return _redis_server_info;
    }

    inline std::pair<std::string, uint16_t> Configuration::get_rest_server_info() const {
        return _rest_server_info;
    }

    inline std::tuple<std::string, int, std::string> Configuration::get_quote_server_info() const {
        return _quote_server_info;
    }

    inline std::pair<std::string, int> Configuration::get_elastic_search_info() const {
        return _elastic_search_info;
    }

    inline std::pair<std::string, int> Configuration::get_permission_server_info() const {
        return _permission_server_info;
    }

    inline uint8_t Configuration::get_trade_client_id() const {
        return _trade_client_id;
    }

    inline uint8_t Configuration::get_quote_client_id() const {
        return _quote_client_id;
    }

    inline std::string Configuration::get_trader_server_key() const {
        return _trader_server_key;
    }

    inline std::string Configuration::get_quote_server_password() const {
        return _quote_server_password;
    }

    inline bool Configuration::get_trade_time_limit() const {
        return _trade_time_limit;
    }

    inline std::string Configuration::get_aivwap_model() const {
        return _aivwap_model;

    }

    inline std::pair<std::string, int> Configuration::get_mysql_server_info() const {
        return _mysql_server_info;
    }

    inline std::tuple<std::string, std::string, std::string> Configuration::get_mysql_db_info() const {
        return _mysql_db_info;
    }

    inline bool Configuration::get_current_store() const {
        return _current_store;
    }

}

#endif //ATS_CONFIGURATION_H

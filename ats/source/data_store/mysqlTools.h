//
// Created by xtgu on 11/4/19.
//

#ifndef MUSE_MYSQLTOOLS_H
#define MUSE_MYSQLTOOLS_H

#include <string>
#include "mysql.h"

namespace ats {
    class MysqlTools {
    private:
        MysqlTools() = default;

        ~MysqlTools();

    public:
        static MysqlTools *get_instance();

        int connect(std::string &ip, std::string &username, std::string &password, std::string &db_name, int port);

        int query(std::string &sql_str);

        int get_query_result();

        void disconnect();

    private:
        void error_catch(std::string error_messages);

    private:
        MYSQL *_connect_handler;


    };
}


#endif //MUSE_MYSQLTOOLS_H

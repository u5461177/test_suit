//
// Created by xtgu on 11/4/19.
//

#include "mysqlTools.h"
#include "../util/gloghelper.h"

namespace ats {
    using namespace std;

    MysqlTools::~MysqlTools() {
        disconnect();
    }

    MysqlTools *MysqlTools::get_instance() {
        static MysqlTools instance;
        return &instance;

    }

    int MysqlTools::connect(string &ip, string &username, string &password, string &db_name, int port) {
        _connect_handler = mysql_init(nullptr);
        auto res = mysql_real_connect(_connect_handler, ip.c_str(), username.c_str(), password.c_str(), db_name.c_str(),
                                      (unsigned int) port, nullptr, 0);
        if (!res) {
            error_catch("mysql connection fault");
            return -1;
        }
        return 0;
    }


    void MysqlTools::disconnect() {
        mysql_close(_connect_handler);
    }

    int MysqlTools::query(std::string &sql_str) {
        int res = mysql_query(_connect_handler, sql_str.c_str());
        if (res) {
            error_catch("mysql query fault");
            return -1;
        }
        return 0;
    }

    int MysqlTools::get_query_result() {
        MYSQL_RES *result;
        MYSQL_ROW sql_row;
        result = mysql_store_result(_connect_handler);
        if (result) {
            while (true) {
                sql_row = mysql_fetch_row(result);
                if (sql_row == nullptr) {
                    return 0;
                }
                //TODO
                cout << "| " << sql_row[1] << " | " << sql_row[2] << " |" << endl;
            }
        } else {
            error_catch("get query result fault");
            return -1;

        }
    }

    void MysqlTools::error_catch(string error_messages) {
        string err_string = mysql_error(_connect_handler);
        if (err_string.empty()) {
            LOG(ERROR) << error_messages << endl;
        } else {
            LOG(ERROR) << err_string << endl;
        }

    }


}

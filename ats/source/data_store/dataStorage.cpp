//
// Created by xtgu on 11/4/19.
//

#include "dataStorage.h"
#include "redisTools.h"
#include "mysqlTools.h"
#include "../configuration.h"
#include "../util/gloghelper.h"

namespace ats {
    using namespace std;
    using namespace nlohmann;

    DataStorage *DataStorage::get_instance() {
        static DataStorage instance;
        return &instance;
    }


    void DataStorage::start() {
        auto mysql_info = Configuration::instance().get_mysql_server_info();
        auto db_info = Configuration::instance().get_mysql_db_info();
        auto redis_info = Configuration::instance().get_redis_server_info();
        RedisTools::get_instance()->connect(redis_info.first, redis_info.second);
        RedisTools::get_instance()->subscribed_channel();
        int res = MysqlTools::get_instance()->connect(mysql_info.first, get<0>(db_info), get<1>(db_info),
                get<2>(db_info), mysql_info.second);
        if (0 == res) {
            LOG(INFO) << "Mysql connect successful!";

        }
    }


    void DataStorage::on_redis_messages(nlohmann::json messages) {
        string sql = parse_task_json(messages);
        if (!sql.empty()) {
            MysqlTools::get_instance()->query(sql);
        }

    }

    string DataStorage::parse_task_json(json json_messages) {
        string sql = {};
        try {
            auto task_id = json_messages["task_id"].get<int>();
            auto status = json_messages["status"].get<string>();
            auto report_type = json_messages["report_type"].get<string>();
            auto task_type = json_messages["task_type"].get<string>();
            auto unique_id = json_messages["unique_id"].get<int>();
            auto start_time = json_messages["start_time"].get<string>();
            auto end_time = json_messages["end_time"].get<string>();
            auto ticker = json_messages["ticker"].get<string>();
            auto quantity = json_messages["quantity"].get<int>();
            auto amount = json_messages["amount"].get<double>();
            auto side = json_messages["side"].get<string>();
            auto progress = json_messages["progress"].get<float>();
            auto order_num = json_messages["order_num"].get<int>();
            auto trade_num = json_messages["trade_num"].get<int>();
            auto avg_traded_price = json_messages["avg_traded_price"].get<float>();
            auto avg_market_price = json_messages["avg_market_price"].get<float>();
            auto traded_quantity = json_messages["traded_quantity"].get<int>();
            auto pending_quantity = json_messages["pending_quantity"].get<int>();
            auto traded_amount = json_messages["traded_amount"].get<double>();
            auto pending_amount = json_messages["pending_amount"].get<double>();
            auto predicted_amount = json_messages["predicted_amount"].get<double>();
            auto left_amount = json_messages["left_amount"].get<double>();
            auto left_quantity = json_messages["left_quantity"].get<int>();
            auto bps = json_messages["bps"].get<float>();
            auto passive_ratio = json_messages["passive_ratio"].get<float>();
            auto remark = json_messages["remark"].get<string>();
            auto report_date = json_messages["report_date"].get<string>();
            auto report_time = json_messages["report_time"].get<string>();
            auto username = json_messages["username"].get<string>();
            auto decryptedMac = json_messages["decryptedMac"].get<string>();
            ostringstream str_stream;
            str_stream << "INSERT INTO task"
                    << "(`task_id`,`status`,`report_type`  ,`task_type` , `unique_id`,`start_time` ,`end_time`"
                       ", `ticker`,`quantity` , `amount`,`side` , `progress` ,`order_num` ,`trade_num`"
                       ",`avg_traded_price`,`avg_market_price`,`traded_quantity`,`pending_quantity`,`traded_amount`"
                       ",`pending_amount`,`predicted_amount`,`left_amount`,`left_quantity`,`bps`,`passive_ratio`,`remark`"
                       ",`report_date`,`report_time`,`username`,`decryptedMac`)"
                       << "VALUE ("
                       << task_id <<","
                       <<"'"<< status <<"',"
                       <<"'"<< report_type <<"',"
                       <<"'"<< task_type <<"',"
                       <<"'"<< unique_id <<"',"
                       <<"'"<< start_time << "',"
                       <<"'"<< end_time << "',"
                       <<"'"<< ticker <<"',"
                       << quantity <<","
                       << amount <<","
                       <<"'" << side <<"',"
                       <<"'"<< progress << "',"
                       << order_num <<","
                       << trade_num <<","
                       << avg_traded_price<<","
                       << avg_market_price<<","
                       << traded_quantity <<","
                       << pending_quantity << ","
                       << traded_amount << ","
                       << pending_amount <<","
                       << predicted_amount<<","
                       << left_amount << ","
                       << left_quantity << ","
                       << bps << ","
                       << "'" << passive_ratio << "',"
                       << "'" << remark << "',"
                       << "'" << report_date << "',"
                       << "'" << report_time <<"',"
                       << "'" << username << "',"
                       << "'" << decryptedMac << "')";
            sql = str_stream.str();
        } catch (const json::exception &e) {
            ErrorCategory error (ErrorCode::WRONG_FORMAT);
            LOG(INFO) << error.message();
        }
        return sql;
    }
}

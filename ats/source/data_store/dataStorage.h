//
// Created by xtgu on 11/4/19.
//

#ifndef MUSE_DATASTORAGE_H
#define MUSE_DATASTORAGE_H

#include "cpp_redis/cpp_redis"
#include "mysql.h"
#include <json.hpp>


namespace ats {

    class DataStorage final {


    private:
        DataStorage() = default;

        ~DataStorage() = default;

    private:
        std::string parse_task_json(nlohmann::json json_messages);


    public:
        static DataStorage *get_instance();

        void start();

        void on_redis_messages(nlohmann::json messages);


    };
}


#endif //MUSE_DATASTORAGE_H

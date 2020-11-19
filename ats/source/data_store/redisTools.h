//
// Created by xtgu on 11/4/19.
//

#ifndef MUSE_REDISTOOLS_H
#define MUSE_REDISTOOLS_H

#include <cpp_redis/cpp_redis>

namespace ats {
    class RedisTools final {
    private:
        RedisTools() = default;

        ~RedisTools();

    public:
        static RedisTools *get_instance();

        void connect(std::string server_ip, int server_port);

        void subscribed_channel();

    private:
        void on_task_channel(const std::string &channel, const std::string &msg);

        void disconnect();

    private:
        cpp_redis::subscriber _redis_subscriber;
        cpp_redis::client _redis_client;

    };
}


#endif //MUSE_REDISTOOLS_H

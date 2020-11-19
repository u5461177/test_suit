//
// Created by Rong Zhou on 2018-08-03.
//

#ifndef ATS_REDISSERVER_H
#define ATS_REDISSERVER_H

#include "singleton.h"
#include <cpp_redis/cpp_redis>
#include "task.h"
#include "taskreport.h"

namespace ats {

    using Ticker = std::pair<std::string, MarketType>;
    using Quantity = std::pair<int64_t, SideType>;

    class RedisServer : public Singleton<RedisServer> {
    public:
        RedisServer() = default;

        ~RedisServer() final;

    public:
        void connect();

        void disconnect();

        void task_status_reset();

        void on_user_created(const std::string &username);

        void on_user_destroyed(const std::string &username);

        void on_user_connected(const std::string &username, const std::string &password, const std::string &server_ip,
                               uint64_t xtp_session_id);

        void on_user_disconnected(const std::string &username);

        void on_user_login(const std::string &username, const std::string &cli_session_id);

        void on_user_logout(const std::string &username, const std::string &cli_session_id);

        void task_id_init();

        uint32_t task_id_create();

        void
        on_task_created(uint32_t task_id, const std::string &owner, const Ticker &ticker, const Quantity &quantity);

        void on_task_destroyed(uint32_t task_id);

        void on_report_destroyed(uint32_t task_id);

        void on_task_status_changed(uint32_t task_id, Status task_status);

        void on_task_report_changed(const std::string &username, Type task_type, uint32_t task_id,
                                    std::shared_ptr<TaskReport> task_report, const std::string &user_mac);

        void on_user_add_task(const std::string &username, uint32_t task_id);

        void on_user_remove_task(const std::string &username, uint32_t task_id);

        void on_user_add_channel(const std::string &username, const std::string &channel_id);

        void on_user_remove_channel(const std::string &username, const std::string &channel_id);

        std::future<cpp_redis::reply> read_hashes(const std::string &hash);

        void write_stream(const std::string &stream, const std::vector<std::string> &data);

        std::future<cpp_redis::reply>
        read_stream(const std::string &report_type, const std::string &username, uint32_t task_id, size_t count_num = 0,
                    uint32_t begin = 0, uint32_t end = 0);

        std::future<cpp_redis::reply> read_sets(const std::string &set_name);

        void on_time_write(const std::string &username, const std::map<uint64_t, std::vector<std::string>> &time_result,
                           uint32_t task_id);

        void _send_task_report(const std::string &username, Type task_type, uint32_t task_id,
                std::shared_ptr<TaskReport> task_report, const std::string &user_mac);

    private:
        void on_admin_channel(const std::string &channel, const std::string &msg);

        void on_user_channel(const std::string &channel, const std::string &msg);

        void on_task_channel(const std::string &channel, const std::string &msg);

    private:
        cpp_redis::client _redis_client;
        cpp_redis::subscriber _redis_subscriber;
    };

}

#endif //ATS_REDISSERVER_H

//
// Created by Rong Zhou on 2018-08-03.
//

#include "redisserver.h"
#include <sstream>
#include <iterator>
#include "configuration.h"
#include "usermanager.h"
#include "glog/logging.h"
#include "util/timehelper.h"

namespace ats {

    using namespace std;
    using json = nlohmann::json;
    using namespace std::chrono;
    using time_point = std::chrono::system_clock::time_point;

    RedisServer::~RedisServer() {
        if (_redis_client.is_connected()) {
            disconnect();
        }
    }

    void RedisServer::connect() {
        auto info = Configuration::instance().get_redis_server_info();
        auto server_ip = info.first;
        auto server_port = info.second;
        _redis_client.connect(server_ip, (size_t) server_port,
                              [](const string &host, size_t port, cpp_redis::client::connect_state status) {
                                  if (status == cpp_redis::client::connect_state::ok) {
                                      LOG(INFO) << "redis client connect server successfully";
                                  } else if (status == cpp_redis::client::connect_state::dropped) {
                                      LOG(ERROR) << "redis client failed to connect server";
                                  }
                              });
        _redis_subscriber.connect(server_ip, (size_t) server_port,
                                  [](const string &host, size_t port, cpp_redis::subscriber::connect_state status) {
                                      if (status == cpp_redis::subscriber::connect_state::ok) {
                                          LOG(INFO) << "redis subscriber connect server successfully";
                                      } else if (status == cpp_redis::subscriber::connect_state::dropped) {
                                          LOG(ERROR) << "redis subscriber failed to connect server";
                                      }
                                  });
//        _redis_subscriber.subscribe("task_report",[this](const string &channel, const string &msg){
//
//            LOG(INFO) << "Subscribe task_report: " << msg ;
//        });

//        _redis_subscriber.subscribe("admin", [this](const string &channel, const string &msg) {
//            on_admin_channel(channel, msg);
//        });
//        _redis_subscriber.subscribe("user", [this](const string &channel, const string &msg) {
//            on_user_channel(channel, msg);
//        });
//        _redis_subscriber.subscribe("task", [this](const string &channel, const string &msg) {
//            on_task_channel(channel, msg);
//        });
// _redis_subscriber.commit();
        task_id_init();
        task_status_reset();
    }

    void RedisServer::disconnect() {
        if (_redis_subscriber.is_connected()) {
            _redis_subscriber.unsubscribe("task_report");
//            _redis_subscriber.unsubscribe("admin");
//            _redis_subscriber.unsubscribe("user");
//            _redis_subscriber.unsubscribe("task");
            _redis_subscriber.disconnect();
        }
        _redis_client.disconnect();
    }

    void RedisServer::task_status_reset() {
        LOG(INFO) << "start to reset task status";
        auto reply = read_sets("tasks");
        vector<uint32_t> task_ids{};
        auto reply_res = reply.get().as_array();
        if (!reply_res.empty()) {
            for (auto &task : reply_res) {
                auto key = "task." + task.as_string();
                auto res = _redis_client.hget(key, "status");
                _redis_client.commit();
                auto status_rtn = res.get().as_string();
                if ("FINISHED" != status_rtn and "STOPPED" != status_rtn) {
                    _redis_client.hset(key, "status", "STOPPED");
                    _redis_client.commit();
                    LOG(WARNING) << "task status was reset to STOPPED | original status: " << status_rtn
                                 << " | task is: " << key;
                }
            }
        }
    }

    void RedisServer::on_user_created(const string &username) {
        _redis_client.sadd("users", {username});
        _redis_client.commit();
    }

    void RedisServer::on_user_destroyed(const string &username) {
        _redis_client.srem("users", {username});
        _redis_client.commit();
    }

    void RedisServer::on_user_connected(const string &username, const string &password, const string &server_ip,
                                        uint64_t xtp_session_id) {
        auto key = "user." + username;
        _redis_client.hset(key, "password", password);
        _redis_client.hset(key, "server_ip", server_ip);
        _redis_client.hset(key, "xtp_session_id", to_string(xtp_session_id));
        _redis_client.commit();
    }

    void RedisServer::on_user_disconnected(const string &username) {
        auto key = "user." + username;
        _redis_client.del({key});
        _redis_client.commit();
    }

    void RedisServer::on_user_login(const string &username, const string &cli_session_id) {
        auto key = username + ".client_sessions";
        _redis_client.sadd(key, {cli_session_id});
        _redis_client.commit();
    }

    void RedisServer::on_user_logout(const string &username, const string &cli_session_id) {
        auto key = username + ".client_sessions";
        _redis_client.srem(key, {cli_session_id});
        _redis_client.commit();
    }

    void RedisServer::task_id_init() {
        auto reply = _redis_client.keys("task_id");
        _redis_client.commit();
        auto flag = reply.get().as_array();
        if (flag.empty()) {
            uint32_t init_task_id = 1000000000;
            _redis_client.set("task_id", to_string(init_task_id));
            _redis_client.commit();
            LOG(INFO) << "task id was initialized | init task id: " << init_task_id;
        }
    }

    uint32_t RedisServer::task_id_create() {
        auto reply = _redis_client.incr("task_id");
        _redis_client.commit();
        auto task_id = (uint32_t) reply.get().as_integer();
        LOG(INFO) << "new task id was created | task id: " << task_id;
        return task_id;
    }

    void RedisServer::on_task_created(uint32_t task_id, const string &owner, const Ticker &ticker,
                                      const Quantity &quantity) {
        _redis_client.sadd("tasks", {to_string(task_id)});
        auto key = "task." + to_string(task_id);
        _redis_client.hset(key, "status", Task::status_to_string(Status::READY));
        _redis_client.commit();
    }

    void RedisServer::on_task_destroyed(uint32_t task_id) {
        _redis_client.srem("tasks", {to_string(task_id)});
        auto key = "task." + to_string(task_id);
        _redis_client.del({key});
        _redis_client.del({"task_id"});
        _redis_client.commit();
    }

    void RedisServer::on_report_destroyed(uint32_t task_id) {
        auto key = "minute_report." + to_string(task_id);
        _redis_client.del({key});
        _redis_client.del({"order_info." + to_string(task_id)});
        _redis_client.del({"trade_report." + to_string(task_id)});
        _redis_client.commit();
    }

    void RedisServer::on_task_status_changed(uint32_t task_id, Status task_status) {
        auto key = "task." + to_string(task_id);
        _redis_client.hset(key, "status", Task::status_to_string(task_status));
        _redis_client.commit();
    }

    void RedisServer::on_task_report_changed(const string &username, Type task_type, uint32_t task_id,
                                             shared_ptr<TaskReport> task_report, const string &user_mac) {
        auto key = "task." + to_string(task_id);
        auto record = TaskReport::to_vector(*task_report);
        record.emplace_back(make_pair("report_date", date_to_string(system_clock::now())));
        record.emplace_back(make_pair("report_time", time_point_to_string(system_clock::now())));
        record.emplace_back(make_pair("username", username));
        record.emplace_back(make_pair("decryptedMac", user_mac));
        record.emplace_back(make_pair("task_type", Task::type_to_string(task_type)));
        _redis_client.hmset(key, record);
        _redis_client.commit();

        _send_task_report(username, task_type, task_id, task_report, user_mac);

    }

    void RedisServer::_send_task_report(const string &username, Type task_type, uint32_t task_id,
                                       shared_ptr<TaskReport> task_report, const string &user_mac) {
        auto task_report_json = TaskReport::to_json(*task_report);
        task_report_json["report_date"] = date_to_string(system_clock::now());
        task_report_json["report_time"] = time_point_to_string(system_clock::now());
        task_report_json["username"] = username;
        task_report_json["decryptedMac"] = user_mac;
        task_report_json["task_type"] = Task::type_to_string(task_type);
        string record_string = task_report_json.dump();
        _redis_client.publish("task_report", record_string);
        _redis_client.commit();


    }

    void RedisServer::on_user_add_task(const string &username, uint32_t task_id) {
        auto key = username + ".tasks";
        _redis_client.sadd(key, {to_string(task_id) + "," + date_to_string(system_clock::now())});
        _redis_client.commit();
    }

    void RedisServer::on_user_remove_task(const string &username, uint32_t task_id) {
        auto key = username + ".tasks";
        _redis_client.srem(key, {to_string(task_id) + "," + date_to_string(system_clock::now())});
        _redis_client.commit();
    }

    void RedisServer::on_user_add_channel(const string &username, const string &channel_id) {
        auto key = username + ".channels";
        _redis_client.sadd(key, {channel_id + "," + date_to_string(system_clock::now())});
        _redis_client.commit();
    }

    void RedisServer::on_user_remove_channel(const string &username, const string &channel_id) {
        auto key = username + ".channels";
        _redis_client.srem(key, {channel_id + "," + date_to_string(system_clock::now())});
        _redis_client.commit();
    }

    future<cpp_redis::reply> RedisServer::read_hashes(const string &hash) {
        vector<string> command{"hgetall", hash};
        auto reply = _redis_client.send(command);
        _redis_client.commit();
        return reply;
    }

    void RedisServer::write_stream(const string &stream, const vector<string> &data) {
        vector<string> command{"xadd", stream, "*"};
        command.insert(command.end(), make_move_iterator(data.begin()), make_move_iterator(data.end()));
        _redis_client.send(command);
        _redis_client.commit();
    }

    future<cpp_redis::reply>
    RedisServer::read_stream(const string &stream, const string &username, uint32_t task_id, size_t count_num,
                             uint32_t begin, uint32_t end) {
        vector<string> command{"xrange", stream, "-", "+"};
        auto reply = _redis_client.send(command);
        _redis_client.commit();
        return reply;
    }

    future<cpp_redis::reply> RedisServer::read_sets(const string &set_name) {
        vector<string> command{"smembers", set_name};
        auto reply = _redis_client.send(command);
        _redis_client.commit();
        return reply;
    }

    void RedisServer::on_admin_channel(const string &channel, const string &msg) {
        istringstream iss(msg);
        vector<string> tokens{istream_iterator<string>{iss}, istream_iterator<string>()};
        auto command = tokens[0];
        size_t size = tokens.size();
        if (command == "reset" and size == 1) {
            UserManager::instance().reset();
        } else {
            LOG(WARNING) << "unrecognized admin command or parameter error";
        }
    }

    void RedisServer::on_user_channel(const string &channel, const string &msg) {
        istringstream iss(msg);
        vector<string> tokens{istream_iterator<string>{iss}, istream_iterator<string>()};
        auto command = tokens[0];
        size_t size = tokens.size();
        if (command == "login" and size == 4) {
            const auto &username = tokens[1];
            const auto &password = tokens[2];
            const auto &server_ip = tokens[3];
            auto server_port = stoi(tokens[4]);
            string  user_mac = {};
            UserManager::instance().user_login(username, password, server_ip, server_port,user_mac);
        } else if (command == "logout" and size == 2) {
            auto cli_session_id = tokens[1];
            UserManager::instance().user_logout(cli_session_id);
        } else {
            LOG(WARNING) << "unrecognized user command or parameter error";
        }
    }

    void RedisServer::on_task_channel(const string &channel, const string &msg) {
        istringstream iss(msg);
        vector<string> tokens{istream_iterator<string>{iss}, istream_iterator<string>()};
        auto command = tokens[0];
        size_t size = tokens.size();
        if (command == "start") {

        } else if (command == "pause" and size == 3) {
            auto &cli_session_id = tokens[1];
            auto &task_id = tokens[2];
            if (auto user = UserManager::instance().find_user(cli_session_id)) {
                if (auto task = user->find_task((uint32_t) stoi(task_id))) {
                    task->pause(cli_session_id);
                }
            }
        } else if (command == "resume" and size == 3) {
            auto &cli_session_id = tokens[1];
            auto &task_id = tokens[2];
            if (auto user = UserManager::instance().find_user(cli_session_id)) {
                if (auto task = user->find_task((uint32_t) stoi(task_id))) {
                    task->resume(cli_session_id);
                }
            }
        } else if (command == "stop" and size == 3) {
            auto &cli_session_id = tokens[1];
            auto &task_id = tokens[2];
            if (auto user = UserManager::instance().find_user(cli_session_id)) {
                if (auto task = user->find_task((uint32_t) stoi(task_id))) {
                    task->stop(cli_session_id);
                }
            }
        } else {
            LOG(WARNING) << "unrecognized user command or parameter error";
        }
    }

    void RedisServer::on_time_write(const string &username, const map<uint64_t, vector<string>> &time_result,
                                    uint32_t task_id) {
        auto key = "time_result." + get_current_date() + "." + to_string(task_id);
        for (const auto &xtp_order_id : time_result) {
            vector<string> command{"xadd", key, "*"};
            command.insert(command.end(), make_move_iterator(xtp_order_id.second.begin()),
                           make_move_iterator(xtp_order_id.second.end()));
            _redis_client.send(command);
        }
        _redis_client.commit();
    }

}
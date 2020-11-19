//
// Created by xtgu on 11/4/19.
//

#include "redisTools.h"
#include "../util/gloghelper.h"
#include "dataStorage.h"

namespace ats {
    using namespace std;

    RedisTools::~RedisTools() {
        if (_redis_client.is_connected()) {
            disconnect();
        }

    }

    RedisTools *RedisTools::get_instance() {
        static RedisTools instance;
        return &instance;
    }

    void RedisTools::connect(string server_ip, int server_port) {
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

    }

    void RedisTools::subscribed_channel() {
        _redis_subscriber.subscribe("task_report", [this](const string &channel, const string &msg) {
            on_task_channel(channel, msg);
        });
        _redis_subscriber.commit();

    }

    void RedisTools::on_task_channel(const std::string &channel, const std::string &msg) {
        auto messages_data = nlohmann::json::parse(string(msg.begin(), msg.end()));
        DataStorage::get_instance()->on_redis_messages(messages_data);


    }

    void RedisTools::disconnect() {
        if (_redis_subscriber.is_connected()) {
            _redis_subscriber.unsubscribe("task_report");
            _redis_subscriber.disconnect();
        }
        _redis_client.disconnect();

    }


}
//
// Created by Rong Zhou on 2018-08-03.
//

#include "usermanager.h"
#include <thread>
#include "types/errorcode.h"


namespace ats {

    using namespace std;

    void UserManager::reset() {
        lock_guard<shared_mutex> guard(_shared_mutex);
        _cli_session_users.clear();
        _xtp_session_users.clear();
    }

    pair<ErrorCategory, string>
    UserManager::user_login(const string &username, const string &password, const string &server_ip, int server_port,
            const std::string &user_mac) {
        auto user = find_or_create_user(username);
        auto result = user->login(password, server_ip, server_port, user_mac);
        if (result.first.get_error_code() == ErrorCode::NO_ERROR) {
            lock_guard<shared_mutex> guard(_shared_mutex);
            auto cli_session_id = result.second;
            auto xtp_session_id = user->get_xtp_session_id();
            _cli_session_users[cli_session_id] = user;
            _xtp_session_users[xtp_session_id] = user;
        }
        return result;
    }

    ErrorCategory UserManager::user_logout(const string &cli_session_id) {
        lock_guard<shared_mutex> guard(_shared_mutex);
        auto i = _cli_session_users.find(cli_session_id);
        if (i not_eq _cli_session_users.end()) {
            auto user = i->second;
            auto old_xtp_session_id = user->get_xtp_session_id();
            auto result = user->logout(cli_session_id);
            auto new_xtp_session_id = user->get_xtp_session_id();
            _cli_session_users.erase(i);
            if (new_xtp_session_id == 0) {
                _xtp_session_users.erase(old_xtp_session_id);

            }
            return result;
        }
        return ErrorCategory(ErrorCode::WRONG_SESSION_ID);
    }

    shared_ptr<User> UserManager::find_user(const string &cli_session_id) {
        shared_lock<shared_mutex> guard(_shared_mutex);
        auto i = _cli_session_users.find(cli_session_id);
        return i not_eq _cli_session_users.end() ? i->second : nullptr;
    }

    shared_ptr<User> UserManager::find_user(uint64_t xtp_session_id) {
        shared_lock<shared_mutex> guard(_shared_mutex);
        auto i = _xtp_session_users.find(xtp_session_id);
        return i not_eq _xtp_session_users.end() ? i->second : nullptr;
    }

    size_t UserManager::count_user() {
        shared_lock<shared_mutex> guard(_shared_mutex);
        return _xtp_session_users.size();
    }

    size_t UserManager::count_client_session() {
        shared_lock<shared_mutex> guard(_shared_mutex);
        return _cli_session_users.size();
    }

    std::shared_ptr<User> UserManager::find_or_create_user(const string &username) {
        shared_lock<shared_mutex> guard(_shared_mutex);
        auto i = find_if(_xtp_session_users.begin(), _xtp_session_users.end(), [=](const auto &item) {
            auto user = item.second;
            return user->get_username() == username;
        });
        if (i not_eq _xtp_session_users.end()) {
            return i->second;
        }
        auto user = make_shared<User>(username);
        user->add_channel(make_shared<RedisChannel>(user));
        return user;
    }

    void UserManager::on_disconnected(uint64_t xtp_session_id) {

        if (shared_ptr<User> user = find_user(xtp_session_id)) {
            user->on_disconnected();
            auto delay = 5s;
            int times = 10;
            for(int i =0; i<times; i++) {
                LOG(INFO) << "try to reconnect" << endl;
                auto result = user->reconnect();
                LOG(INFO) << "after reconnect function";
                if (result.first.get_error_code() == ErrorCode::WRONG_PASSWORD or
                result.first.get_error_code() == ErrorCode::USER_NOT_EXIST) {
                    auto user_sessions = user->get_cli_session_ids();
                    {
                        lock_guard<shared_mutex> guard(_shared_mutex);
                        _xtp_session_users.erase(xtp_session_id);
                        for_each(user_sessions.begin(), user_sessions.end(), [&](const string &cli) {
                            _cli_session_users.erase(cli);
                        });
                    }
                    user->relogin();
                }

                if (result.first.get_error_code() == ErrorCode::NO_ERROR) {
                    LOG(INFO) << "Change xtp_session_id";
                    auto new_xtp_session_id = user->get_xtp_session_id();
                    {
                        lock_guard<shared_mutex> guard(_shared_mutex);
                        _xtp_session_users[new_xtp_session_id] = user;
                        _xtp_session_users.erase(xtp_session_id);
                    }
                    LOG(INFO) << "Finish change xtp_session_id";
                    return;
                }
                this_thread::sleep_for(delay);
                delay *= 2;
            }
            auto tasks = user->get_tasks();
            for(auto &task: tasks){
                task->stop();
            }

        }
    }

}
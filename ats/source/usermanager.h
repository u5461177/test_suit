//
// Created by Rong Zhou on 2018-08-03.
//

#ifndef ATS_USERMANAGER_H
#define ATS_USERMANAGER_H

#include "singleton.h"
#include <map>
#include <shared_mutex>
#include <system_error>
#include "user.h"

namespace ats {

    class UserManager : public Singleton<UserManager> {
    public:
        UserManager() = default;

        ~UserManager() final = default;

    public:
        void reset();

        std::pair<ErrorCategory, std::string>
        user_login(const std::string &username, const std::string &password, const std::string &server_ip,
                   int server_port, const std::string &user_mac);

        ErrorCategory user_logout(const std::string &cli_session_id);

        std::shared_ptr<User> find_user(const std::string &cli_session_id);

        std::shared_ptr<User> find_user(uint64_t xtp_session_id);

        size_t count_user();

        size_t count_client_session();

        void on_disconnected(uint64_t xtp_session_id);

        std::shared_ptr<User> find_or_create_user(const std::string &username);

    private:
        std::map<std::string, std::shared_ptr<User>> _cli_session_users;
        std::map<uint64_t, std::shared_ptr<User>> _xtp_session_users;
        std::shared_mutex _shared_mutex;
    };

}

#endif //ATS_USERMANAGER_H
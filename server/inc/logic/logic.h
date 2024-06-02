#pragma once
#include "logic/user.h"

#include "ai_client/ai.h"
#include "utils/config.h"

#include "error_codes.h"
#include "ext/json.h"

#include <memory>
#include <string>
#include <unordered_map>

namespace Logic {
    class CoreLogic {
    public:
        CoreLogic();
        ERROR_CODE authUser(const std::string& id);
        ERROR_CODE registerUser(const std::string& id);
        ERROR_CODE send(const std::string& id, std::string_view instruction, std::string &response);
        ERROR_CODE tune(const std::string& id);
        ERROR_CODE boost(const std::string& id);

        void save();

    private:
        std::shared_ptr<User> getUser(const UserKey& key);

    private:
        std::mutex m_usersLock;
        std::unordered_map<UserKey, std::shared_ptr<User>> m_users;
    };
}// namespace Logic
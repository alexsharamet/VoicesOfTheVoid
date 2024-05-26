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
        ERROR_CODE authUser(std::string_view name);
        ERROR_CODE registerUser(std::string_view name);
        ERROR_CODE send(std::string_view name, std::string_view instruction, std::string &response);
        ERROR_CODE tune(std::string_view name);
        ERROR_CODE boost(std::string_view name);

        void save();

    private:
        std::shared_ptr<User> getUser(UserKey);

    private:
        std::shared_ptr<AI::AI> m_ai;//TODO: shared resource fix data race

        std::mutex m_usersLock;
        std::unordered_map<UserKey, std::shared_ptr<User>> m_users;
    };
}// namespace Logic
#pragma once
#include "logic/user.h"

#include "ai_client/ai.h"
#include "utils/config.h"

#include "ext/json.h"

#include <memory>
#include <string>
#include <unordered_map>

namespace Logic {
    class CoreLogic {
    public:
        CoreLogic();
        bool authUser(std::string_view name);
        bool registerUser(std::string_view name);
        std::string send(std::string_view user, std::string_view instruction);

        void save();

    private:
        AI::AI m_ai;//TODO: shared resource fix data race

        std::mutex m_usersLock;
        std::unordered_map<UserKey, std::shared_ptr<User>> m_users;
    };
}// namespace Logic
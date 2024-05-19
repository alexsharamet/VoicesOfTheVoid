#pragma once
#include "logic/user.h"

#include "ai_client/ai.h"

#include "ext/json.h"

#include <map>
#include <string>

namespace Logic {
    class CoreLogic {
    public:
        explicit CoreLogic(nlh::json cnfg);
        bool registerUser(std::string_view name);
        std::string send(std::string_view user, std::string_view instruction);

        [[nodiscard]] nlh::json toJson() const;

    private:
        nlh::json m_cnfg;
        AI::AI m_ai;//TODO: shared resource fix data race

        std::mutex m_usersLock;
        std::map<std::string, User> m_users;
    };
}// namespace Logic
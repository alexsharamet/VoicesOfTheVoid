#include "logic/logic.h"

#include "ext/json.h"
#include "logic/strategy.h"
#include "logic/strategy/ai_strategy.h"

#include <iostream>

namespace Logic {
    CoreLogic::CoreLogic() {
        auto &config = Utils::Config::instance();
        m_ai = std::make_shared<AI::AI>();

        m_ai->setHost(config.getAIHost());
        m_ai->setPort(config.getAIPort());

        auto users = config.getUsers();
        for (const auto &user: users) {
            if (user.contains("name")) {
                m_users.insert({user["name"], std::make_shared<User>(user)});
            } else {
                assert(false);
            }
        }
    }

    void CoreLogic::save() {
        std::vector<nlh::json> users;
        for (const auto &[key, value]: m_users) {
            auto user = value->toJson();
            user["name"] = key;
            users.push_back(std::move(user));
        }
        Utils::Config::instance().setUsers(users);
    }

    std::shared_ptr<User> CoreLogic::getUser(UserKey key) {
        std::lock_guard lock{m_usersLock};
        auto it = m_users.find(key);
        if (it == m_users.end()) {
            return nullptr;
        }
        return it->second;
    }

    ERROR_CODE CoreLogic::authUser(std::string_view name) {
        std::string userName{name};
        std::lock_guard lock{m_usersLock};
        if (m_users.find(std::string{name}) == m_users.end()) {
            return ERROR_CODE::USER_IS_NOT_EXIST;
        }

        return ERROR_CODE::OK;
    }

    ERROR_CODE CoreLogic::registerUser(std::string_view name) {
        std::string userName{name};
        std::lock_guard lock{m_usersLock};

        if (m_users.find(std::string{name}) != m_users.end()) {
            return ERROR_CODE::THIS_NAME_IS_ALREADY_EXIST;
        }

        auto user = std::make_shared<User>();
        user->setGenStrategy(std::make_shared<EchoStrategy>());
        user->setCorruptionStrategy(std::make_shared<EchoStrategy>());
        m_users.emplace(name, user);
        return ERROR_CODE::OK;
    }

    ERROR_CODE CoreLogic::send(std::string_view name, std::string_view instruction, std::string &response) {
        std::shared_ptr<User> user = getUser(std::string{name});
        if (!user) {
            return ERROR_CODE::USER_IS_NOT_EXIST;
        }

        std::unique_lock lock{user->getLockRef(), std::defer_lock};
        if (!lock.try_lock()) {
            return ERROR_CODE::USER_IS_BUSY;
        }

        response = user->ask(std::string{instruction});

        return ERROR_CODE::OK;
    }

    ERROR_CODE CoreLogic::tune(std::string_view name) {
        std::shared_ptr<User> user = getUser(std::string{name});
        if (!user) {
            return ERROR_CODE::USER_IS_NOT_EXIST;
        }

        std::unique_lock lock{user->getLockRef(), std::defer_lock};
        if (!lock.try_lock()) {
            return ERROR_CODE::USER_IS_BUSY;
        }

        user->setGenStrategy(std::make_shared<AIStrategy>(m_ai));

        return ERROR_CODE::OK;
    }

    ERROR_CODE CoreLogic::boost(std::string_view name) {
        std::shared_ptr<User> user = getUser(std::string{name});
        if (!user) {
            return ERROR_CODE::USER_IS_NOT_EXIST;
        }

        std::unique_lock lock{user->getLockRef(), std::defer_lock};
        if (!lock.try_lock()) {
            return ERROR_CODE::USER_IS_BUSY;
        }

        user->changeWeight(10);//TODO some logic

        return ERROR_CODE::OK;
    }
}// namespace Logic
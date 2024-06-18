#include "logic/logic.h"

#include "ext/json.h"
#include "logic/strategy/ai_strategy.h"
#include "logic/strategy/core_corrupt_stategy.h"
#include "logic/strategy/echo_strategy.h"
#include "logic/strategy/prepared_strategy.h"
#include "utils/random.h"

#include <strategies.h>

#include <iostream>

namespace Logic {
    StrategyType selectStrategy() {
        if (Utils::isHappened(40)) {
            return StrategyType::AI;
        }

        if (Utils::isHappened(70)) {
            return StrategyType::Prepared;
        }

        return StrategyType::Echo;
    }

    CoreLogic::CoreLogic() {
        auto &config = Utils::Config::instance();
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

    std::shared_ptr<User> CoreLogic::getUser(const UserKey &key) {
        std::lock_guard lock{m_usersLock};
        auto it = m_users.find(key);
        if (it == m_users.end()) {
            return nullptr;
        }
        return it->second;
    }

    ERROR_CODE CoreLogic::authUser(const std::string &id, StrategyType &type) {
        std::lock_guard lock{m_usersLock};
        auto it = m_users.find(id);
        if (it == m_users.end()) {
            return ERROR_CODE::USER_IS_NOT_EXIST;
        }

        type = it->second->getStrategyType();
        return ERROR_CODE::OK;
    }

    ERROR_CODE CoreLogic::registerUser(const std::string &id, const std::string &name) {
        std::lock_guard lock{m_usersLock};

        if (m_users.find(id) != m_users.end()) {
            return ERROR_CODE::THIS_NAME_IS_ALREADY_EXIST;
        }

        auto user = std::make_shared<User>(name);
        user->setCorruptionStrategy(std::make_shared<CoreCorruptStrategy>());
        m_users.emplace(id, user);
        return ERROR_CODE::OK;
    }

    ERROR_CODE CoreLogic::isFinished(const std::string &id, bool &finished) {
        std::shared_ptr<User> user = getUser(id);
        if (!user) {
            return ERROR_CODE::USER_IS_NOT_EXIST;
        }

        std::unique_lock lock{user->getLockRef(), std::defer_lock};
        if (!lock.try_lock()) {
            return ERROR_CODE::USER_IS_BUSY;
        }

        if (user->getStrategyType() == StrategyType::AI && user->getWeight() == 0) {
            finished = true;
        }

        return ERROR_CODE::OK;
    }

    ERROR_CODE CoreLogic::send(const std::string &id, const std::string &instruction, std::string &response) {
        std::cout << "CoreLogic: send" << std::endl;
        std::shared_ptr<User> user = getUser(id);
        if (!user) {
            return ERROR_CODE::USER_IS_NOT_EXIST;
        }

        std::unique_lock lock{user->getLockRef(), std::defer_lock};
        if (!lock.try_lock()) {
            return ERROR_CODE::USER_IS_BUSY;
        }

        std::cout << "CoreLogic: user ask" << std::endl;
        response = user->ask(instruction);

        return ERROR_CODE::OK;
    }

    ERROR_CODE CoreLogic::tune(const std::string &id, StrategyType &type) {
        std::cout << "tune" << std::endl;
        std::shared_ptr<User> user = getUser(id);
        if (!user) {
            return ERROR_CODE::USER_IS_NOT_EXIST;
        }

        std::unique_lock lock{user->getLockRef(), std::defer_lock};
        if (!lock.try_lock()) {
            return ERROR_CODE::USER_IS_BUSY;
        }

        type = selectStrategy();

        switch (type) {
            case StrategyType::AI:
                user->setGenStrategy(std::make_shared<AIStrategy>());
                break;
            case StrategyType::Echo:
                user->setGenStrategy(std::make_shared<EchoStrategy>());
                break;
            case StrategyType::Prepared:
                user->setGenStrategy(std::make_shared<PreparedStrategy>());
                break;
        }

        user->clearWeight();

        return ERROR_CODE::OK;
    }

    ERROR_CODE CoreLogic::boost(const std::string &id) {
        std::cout << "boost" << std::endl;
        std::shared_ptr<User> user = getUser(id);
        if (!user) {
            return ERROR_CODE::USER_IS_NOT_EXIST;
        }

        std::unique_lock lock{user->getLockRef(), std::defer_lock};
        if (!lock.try_lock()) {
            return ERROR_CODE::USER_IS_BUSY;
        }

        user->changeWeight();

        return ERROR_CODE::OK;
    }
}// namespace Logic
#include "logic/logic.h"

#include "ext/json.h"
#include "logic/strategy.h"
#include "logic/strategy/ai_strategy.h"

#include <iostream>

namespace Logic {
    void selectStrategy(User &user) {
        static auto multiplier = (static_cast<int>(StrategyTypes::COUNT) - 1) / static_cast<float>(RAND_MAX);
        auto selected = static_cast<StrategyTypes>(static_cast<int>(std::rand() * multiplier));
        std::cout << "Selected strategy: " << static_cast<int>(selected) << std::endl;
        switch (selected) {
            case StrategyTypes::AI:
                user.setGenStrategy(std::make_shared<AIStrategy>());
                break;
            default:
                user.setGenStrategy(std::make_shared<EchoStrategy>());
        }
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

    ERROR_CODE CoreLogic::authUser(const std::string &id) {
        std::lock_guard lock{m_usersLock};
        if (m_users.find(id) == m_users.end()) {
            return ERROR_CODE::USER_IS_NOT_EXIST;
        }

        return ERROR_CODE::OK;
    }

    ERROR_CODE CoreLogic::registerUser(const std::string &id) {
        std::lock_guard lock{m_usersLock};

        if (m_users.find(id) != m_users.end()) {
            return ERROR_CODE::THIS_NAME_IS_ALREADY_EXIST;
        }

        auto user = std::make_shared<User>();
        user->setCorruptionStrategy(std::make_shared<EchoStrategy>());
        selectStrategy(*user);
        m_users.emplace(id, user);
        return ERROR_CODE::OK;
    }

    ERROR_CODE CoreLogic::send(const std::string &id, std::string_view instruction, std::string &response) {
        std::shared_ptr<User> user = getUser(id);
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

    ERROR_CODE CoreLogic::tune(const std::string &id) {
        std::shared_ptr<User> user = getUser(id);
        if (!user) {
            return ERROR_CODE::USER_IS_NOT_EXIST;
        }

        std::unique_lock lock{user->getLockRef(), std::defer_lock};
        if (!lock.try_lock()) {
            return ERROR_CODE::USER_IS_BUSY;
        }

        selectStrategy(*user);

        return ERROR_CODE::OK;
    }

    ERROR_CODE CoreLogic::boost(const std::string &id) {
        std::shared_ptr<User> user = getUser(id);
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
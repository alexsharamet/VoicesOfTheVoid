#include "logic/logic.h"

#include "ext/json.h"
#include <fmt/core.h>


#include <iostream>

namespace {
    nlohmann::json createPromt() {
        nlh::json json;
        json["n"] = 1;
        json["max_context_length"] = 2048;
        json["max_length"] = 150;
        json["rep_pen"] = 1.07;
        json["temperature"] = 0.7;
        json["top_p"] = 0.92;
        json["top_k"] = 100;
        json["top_a"] = 0;
        json["typical"] = 1;
        json["tfs"] = 1;
        json["rep_pen_range"] = 320;
        json["rep_pen_slope"] = 0.7;
        json["sampler_order"] = std::vector<int>{6, 0, 1, 3, 4, 2, 5};
        json["memory"] = "";
        json["genkey"] = "KCPP7178";
        json["min_p"] = 0;
        json["dynatemp_range"] = 0;
        json["dynatemp_exponent"] = 1;
        json["smoothing_factor"] = 0;
        json["banned_tokens"] = nlh::json::array();
        json["render_special"] = false;
        json["presence_penalty"] = 0;
        json["logit_bias"] = nlh::json::object();
        json["quiet"] = true;
        json["stop_sequence"] = std::vector<std::string>{"### Instruction:", "### Response:"};
        json["use_default_badwordsids"] = false;
        json["bypass_eos"] = false;

        return json;
    }
}// namespace

namespace Logic {
    CoreLogic::CoreLogic() {
        auto &config = Utils::Config::instance();

        m_ai.setHost(config.getAIHost());
        m_ai.setPort(config.getAIPort());

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

        m_users.emplace(name, std::make_shared<User>());
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

        static nlh::json BASE_PROMT = createPromt();
        auto json{BASE_PROMT};

        std::string promtString;
        auto &history = user->getHistoryRef();
        for (const auto &promt: history) {
            promtString += fmt::format("### Instruction:\n{}\n### Response:\n{}\n", promt.first, promt.second);
        }
        promtString += fmt::format("### Instruction:\n{}\n### Response:\n", instruction);
        json["prompt"] = promtString;

        const auto promt = json.dump();
        std::cout << "Promt sent: " << promt << std::endl;

        response = m_ai.sendPromtSync(promt);
        user->addPromt({std::string{instruction}, response});

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

        //TODO some logic

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

        //TODO some logic

        return ERROR_CODE::OK;
    }
}// namespace Logic
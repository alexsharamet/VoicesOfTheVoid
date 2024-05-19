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
    CoreLogic::CoreLogic(nlh::json cnfg)
        : m_cnfg(std::move(cnfg)) {

        bool success = true;
        success &= m_cnfg.contains("ai_host");
        success &= m_cnfg.contains("ai_port");
        assert(success);

        m_ai.setHost(m_cnfg["ai_host"]);
        m_ai.setPort(m_cnfg["ai_port"]);

        if (m_cnfg.contains("users")) {
            for (const auto &userJson: m_cnfg["users"]) {
                User user(userJson);
                const auto name = user.getName();
                m_users.insert({name, std::move(user)});
            }
        }
    }

    nlh::json CoreLogic::toJson() const {
        nlh::json users = nlh::json::array();
        for (const auto &[name, user]: m_users) {
            users.push_back(user.toJson());
        }

        auto cnfg{m_cnfg};
        cnfg["users"] = users;

        return cnfg;
    }

    bool CoreLogic::registerUser(std::string_view name) {
        std::string userName{name};
        std::lock_guard lock{m_usersLock};
        if (m_users.find(userName) != m_users.end()) {
            return false;//TODO return enum
        }

        m_users.insert({userName, User{userName}});
        return true;
    }

    std::string CoreLogic::send(std::string_view user, std::string_view instruction) {
        std::unique_lock lock{m_usersLock};
        auto it = m_users.find(std::string{user});
        if (it == m_users.end()) {
            return "User not found";
        }
        lock.unlock();

        static nlh::json BASE_PROMT = createPromt();
        auto json{BASE_PROMT};
        json["prompt"] = fmt::format("\n### Instruction:\n{}\n### Response:\n", instruction);

        const auto promt = json.dump();
        std::cout << "Promt sent: " << promt << std::endl;
        return m_ai.sendPromtSync(promt);
    }
}// namespace Logic
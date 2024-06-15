#include "logic/user.h"

#include "utils/config.h"

#include <iostream>

namespace Logic {
    User::User(std::string name)
        : m_name(std::move(name)) {
    }

    User::User(nlh::json json) {
        if (json.contains("history")) {
            auto history = json["history"];
            for (const auto &item: history) {
                bool success = true;
                success &= item.contains("instruction");
                success &= item.contains("response");
                assert(success);
                if (success) {
                    m_history.push_back(std::make_pair<std::string, std::string>(item["instruction"], item["response"]));
                }
            }
        }
    }

    std::mutex &User::getLockRef() {
        return m_lock;
    }

    void User::setGenStrategy(std::shared_ptr<IStrategy> strategy) {
        m_genStrategy = std::move(strategy);
    }

    void User::setCorruptionStrategy(std::shared_ptr<ICorruptionStrategy> strategy) {
        m_corruptionStrategy = std::move(strategy);
    }

    StrategyType User::getStrategyType() const {
        return m_genStrategy->getType();
    }

    void User::addPromt(Promt promt) {
        m_history.push_back(std::move(promt));
        if (m_history.size() > Utils::Config::instance().getHistorySize()) {
            m_history.pop_front();
        }
    }

    std::string User::ask(const std::string &instruction) {
        auto response = m_corruptionStrategy->corrupt(m_genStrategy->ask(m_history, instruction));
        addPromt({instruction, response});

        return response;
    }

    void User::changeWeight() {
        if (m_corruptionWeight > 0) {
            m_corruptionWeight -= 20;
        }
        m_corruptionStrategy->changeWeight(m_corruptionWeight);
    }

    void User::clearWeight() {
        m_corruptionWeight = 100;
    }

    nlh::json User::toJson() const {
        nlh::json res;
        std::vector<nlh::json> history;
        for (const auto &promt: m_history) {
            nlh::json item;
            item["instruction"] = promt.first;
            item["response"] = promt.second;
            history.push_back(std::move(item));
        }
        res["history"] = history;
        return res;
    }
}// namespace Logic
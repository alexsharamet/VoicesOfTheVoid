#include "logic/user.h"

#include "utils/config.h"

#include <iostream>

namespace Logic {
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

    const PromtHistory &User::getHistoryRef() const {
        return m_history;
    }

    void User::addPromt(Promt promt) {
        m_history.push_back(std::move(promt));
        if (m_history.size() > Utils::Config::instance().getHistorySize()) {
            m_history.pop_front();
        }
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
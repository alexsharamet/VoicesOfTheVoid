#pragma once

#include "ext/json.h"
#include "logic/strategy.h"
#include "promt.h"

#include <deque>
#include <mutex>
#include <string>

namespace Logic {
    using UserKey = std::string;

    class User {
    public:
        explicit User(std::string name);
        explicit User(nlh::json json);
        [[nodiscard]] nlh::json toJson() const;

        std::mutex &getLockRef();
        void setGenStrategy(std::shared_ptr<IStrategy> strategy);
        void setCorruptionStrategy(std::shared_ptr<ICorruptionStrategy> strategy);
        [[nodiscard]] StrategyType getStrategyType() const;

        std::string ask(const std::string &text);
        void changeWeight();
        void clearWeight();

    private:
        void addPromt(Promt promt);

    private:
        std::mutex m_lock;
        std::string m_name;
        PromtHistory m_history;
        std::shared_ptr<IStrategy> m_genStrategy;
        std::shared_ptr<ICorruptionStrategy> m_corruptionStrategy;
        int m_corruptionWeight{100};
    };
}// namespace Logic
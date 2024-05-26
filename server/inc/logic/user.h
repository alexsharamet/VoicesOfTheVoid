#pragma once

#include "ext/json.h"
#include "logic/strategy.h"
#include "promt.h"

#include <deque>
#include <string>

namespace Logic {
    using UserKey = std::string;

    class User {
    public:
        User() = default;
        explicit User(nlh::json json);
        [[nodiscard]] nlh::json toJson() const;

        std::mutex &getLockRef();
        void setGenStrategy(std::shared_ptr<IStrategy> strategy);
        void setCorruptionStrategy(std::shared_ptr<ICorruptionStrategy> strategy);

        std::string ask(std::string text);
        void changeWeight(int weight);

    private:
        std::mutex m_lock;
        std::shared_ptr<IStrategy> m_genStrategy;
        std::shared_ptr<ICorruptionStrategy> m_corruptionStrategy;
    };
}// namespace Logic
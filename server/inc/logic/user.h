#pragma once

#include "ext/json.h"
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

        std::mutex& getLockRef();
        [[nodiscard]] PromtHistory getHistory() const;
        void addPromt(Promt promt);

    private:
        std::mutex m_lock;
        PromtHistory m_history;
    };
}// namespace Logic
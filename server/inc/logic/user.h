#pragma once

#include "ext/json.h"

#include <deque>
#include <string>

namespace Logic {
    class User {
    public:
        explicit User(const std::string& name);
        explicit User(nlh::json json);

        [[nodiscard]] bool operator<(const User& rhs) const noexcept;
        std::string getName() const noexcept;

        nlh::json toJson() const;
    private:
        std::string m_name;
    };
}// namespace Logic
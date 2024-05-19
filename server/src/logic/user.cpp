#include "logic/user.h"

#include <iostream>

namespace Logic {
    User::User(const std::string &name)
        : m_name(name) {
    }


    [[nodiscard]] bool User::operator<(const User &rhs) const noexcept {
        return m_name < rhs.m_name;
    }

    std::string User::getName() const noexcept {
        return m_name;
    }

    User::User(nlh::json json) {
        assert(json.contains("name"));

        m_name = json["name"];
    }

    nlh::json User::toJson() const {
        nlh::json res;
        res["name"] = m_name;
        return res;
    }
}// namespace Logic
#pragma once

#include "ext/json.h"

#include <filesystem>

namespace Utils {
    class Config {
    public:
        static Config &instance();

        void load(std::filesystem::path path);
        void save();

        int getVersion() const;
        std::string getAIHost() const;
        int getAIPort() const;
        int getAITimeout() const;
        int getHistorySize() const;
        int getPort() const;
        std::vector<nlh::json> getUsers() const;

        void setUsers(const std::vector<nlh::json> &users);
        void setVersion(int version);

    private:
        Config() = default;

    private:
        std::filesystem::path m_path;
        nlh::json m_cnfg;
        int m_version{0};
    };
}// namespace Utils
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

    private:
        Config() = default;

    private:
        std::filesystem::path m_path;
        nlh::json m_cnfg;
    };
}// namespace Utils
#include "utils/config.h"

#include <fstream>
#include <iostream>

namespace Utils {
    Config &Config::instance() {
        static Config config;
        return config;
    }

    void Config::load(std::filesystem::path path) {
        m_path = path;
        std::ifstream in(m_path);
        std::string str((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
        m_cnfg = nlh::json::parse(str);

        bool success = true;
        success &= m_cnfg.contains("ai_host");
        success &= m_cnfg.contains("ai_port");
        success &= m_cnfg.contains("port");
        success &= m_cnfg.contains("history_size");

        assert(success);
        if (!success) {
            throw std::runtime_error("Wrong config format");
        }
    }

    void Config::save() {
        std::ofstream out(m_path);
        out << std::setw(4) << m_cnfg;
    }

    std::string Config::getAIHost() const {
        return m_cnfg["ai_host"];
    }

    int Config::getAIPort() const {
        return m_cnfg["ai_port"];
    }

    int Config::getHistorySize() const {
        return m_cnfg["history_size"];
    }

    int Config::getPort() const {
        return m_cnfg["port"];
    }

    void Config::setUsers(const std::vector<nlh::json>& users) {
        m_cnfg["users"] = users;
    }

    std::vector<nlh::json> Config::getUsers() const {
        if (!m_cnfg.contains("users")) {
            return {};
        }

        return m_cnfg["users"];
    }
}// namespace Utils
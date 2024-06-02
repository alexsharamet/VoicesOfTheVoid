#pragma once

#include <httplib.h>
#include <memory>
#include <string>

namespace AI {
    class AI {
    public:
        AI(const std::string &host, int port, int timeout);
        [[nodiscard]] std::string sendPromtSync(std::string_view promt);

    private:
        [[nodiscard]] std::string getURL() const;

    private:
        httplib::Client m_client;
    };
}// namespace AI

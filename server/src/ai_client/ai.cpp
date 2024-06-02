#include "ai_client/ai.h"

#include "ext/json.h"

#include <cassert>
#include <iostream>

namespace {
    std::pair<std::string, int> extractJson(const std::string_view &data, int pos) {
        const int begin = static_cast<int>(data.find('{', pos));
        int count = 1;
        int i = begin + 1;

        while (i < data.length()) {
            if (data[i] == '{') {
                ++count;
            }
            if (data[i] == '}') {
                --count;
            }

            if (count == 0) {
                ++i;
                return {std::string(data.substr(begin, i - begin)), i};
            }

            if (count < 0) {
                throw std::runtime_error("invalid response format");
            }

            ++i;
        }

        return {std::string{}, i};
    }

    std::string parse(std::string buffer) {
        std::cout << "AI response: " << buffer << std::endl;

        std::string res;
        int i = 0;
        std::string data;
        while (i < buffer.length()) {
            std::tie(data, i) = extractJson(buffer, i);
            if (!data.empty()) {
                nlh::json val = nlh::json::parse(data);
                res += val["token"];
            }
        }
        return res;
    }
}// namespace

namespace AI {
    AI::AI(const std::string &host, int port, int timeout)
        : m_client(host, port) {
        m_client.set_connection_timeout(timeout);
        m_client.set_read_timeout(timeout);
        m_client.set_write_timeout(timeout);
    }

    std::string AI::sendPromtSync(std::string_view promt) {
        auto res = m_client.Post("/api/extra/generate/stream", std::string{promt}, "Content-Type: application/json");
        if (res->status != 200) {
            std::cerr << "ai response error";
        }

        return parse(res->body);
    }
}// namespace AI

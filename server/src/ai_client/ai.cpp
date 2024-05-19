#include "ai_client/ai.h"

#include "ext/json.h"

#include <cassert>
#include <iostream>

namespace {
    size_t writeFunc(const char *ptr, size_t size, size_t nMemb, void *userData) {
        std::string &s = *static_cast<std::string *>(userData);
        const auto len = size * nMemb;
        s.reserve(s.length() + len);
        for (auto i = 0; i < len; ++i) {
            s += ptr[i];
        }
        return len;
    }

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
}// namespace

namespace AI {
    static std::weak_ptr<GlobalInstance> GLOBAL_INSTANCE;

    struct GlobalInstance {
        GlobalInstance() {
            curl_global_init(CURL_GLOBAL_ALL);
        }

        ~GlobalInstance() {
            curl_global_cleanup();
        }
    };

    AI::AI() {
        initGlobalInstance();
        m_curl = curl_easy_init();
        if (!m_curl) {
            assert(false);
            throw std::runtime_error("CURL is not inited");
        }
        addHeader();
        curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, writeFunc);
        curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &m_buffer);
    }

    AI::~AI() {
        curl_easy_cleanup(m_curl);
    }

    void AI::initGlobalInstance() {
        m_globalInstance = GLOBAL_INSTANCE.lock();
        if (m_globalInstance == nullptr) {
            m_globalInstance = std::make_shared<GlobalInstance>();
            GLOBAL_INSTANCE = m_globalInstance;
        }
    }

    void AI::addHeader() {
        struct curl_slist *hs = nullptr;
        hs = curl_slist_append(hs, "Content-Type: application/json");
        curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, hs);
    }

    std::string AI::getURL() const {
        return "http://" + m_host + ":" + std::to_string(m_port) + "/api/extra/generate/stream";
    }

    void AI::setHost(const std::string &host) {
        m_host = host;
    }

    void AI::setPort(int port) {
        m_port = port;
    }

    std::string AI::parse() const {
        std::cout << "AI response: " << m_buffer << std::endl;

        std::string res;
        int i = 0;
        std::string data;
        while (i < m_buffer.length()) {
            std::tie(data, i) = extractJson(m_buffer, i);
            if (!data.empty()) {
                nlh::json val = nlh::json::parse(data);
                res += val["token"];
            }
        }
        return res;
    }

    std::string AI::sendPromtSync(std::string_view promt) {
        m_buffer.clear();
        const auto url = getURL();
        curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, promt);

        m_res = curl_easy_perform(m_curl);
        if (m_res != CURLE_OK) {
            std::cerr << curl_easy_strerror(m_res);
        }

        return parse();
    }
}// namespace AI
#pragma once

#include <curl/curl.h>

#include <memory>
#include <string>

namespace AI {
    class GlobalInstance;

    class AI {
    public:
        AI();
        ~AI();

        void setHost(std::string_view host);
        void setPort(int port);

        [[nodiscard]] std::string sendPromtSync(std::string_view promt);

    private:
        void initGlobalInstance();
        void addHeader();
        [[nodiscard]] std::string getURL() const;
        [[nodiscard]] std::string parse() const;

    private:
        CURL *m_curl;
        CURLcode m_res{CURLE_OK};
        std::string m_buffer;
        std::string m_host;
        int m_port{0};

        std::shared_ptr<GlobalInstance> m_globalInstance;
    };
}// namespace AI

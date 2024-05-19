#pragma once
#include <httplib.h>

#include <functional>

namespace Server {
    class HttpServer {
    public:
        HttpServer();
        void listen(int port);

        void setRegisterHandler(std::function<bool(std::string /*name*/)>);
        void setSendHandler(std::function<std::string(std::string_view /*user*/, std::string_view /*instruction*/)> handler);

    private:
        httplib::Server svr;
    };
}// namespace Server
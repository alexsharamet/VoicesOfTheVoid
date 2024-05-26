#pragma once
#include <httplib.h>

#include <functional>

#include "error_codes.h"
#include "ext/json.h"

namespace Server {
    class HttpServer {
    public:
        HttpServer();
        void listen(int port);

        void setAuthHandler(std::function<nlh::json(ERROR_CODE &err, std::string /*name*/, int /*version*/)> handler);
        void setRegisterHandler(std::function<nlh::json(ERROR_CODE &err, std::string /*name*/, int /*version*/)> handler);
        void setSendHandler(std::function<nlh::json(ERROR_CODE &err, std::string_view /*name*/, std::string_view /*instruction*/)> handler);
        void setTuneHandler(std::function<nlh::json(ERROR_CODE &err, std::string_view /*name*/)> handler);
        void setBoostHandler(std::function<nlh::json(ERROR_CODE &err, std::string_view /*name*/)> handler);

    private:
        httplib::Server svr;
    };
}// namespace Server
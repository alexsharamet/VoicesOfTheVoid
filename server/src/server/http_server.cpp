#include "server/http_server.h"
#include "ext/json.h"

namespace {
    bool validateArgs(httplib::Response &res, const nlh::json &jsonReq, const std::vector<std::string> &names) {
        bool success = true;
        for (const auto &name: names) {
            success &= jsonReq.contains(name);
        }

        if (!success) {
            nlh::json jsonRes;
            jsonRes["error"] = static_cast<int>(ERROR_CODE::WRONG_REQUEST_ARGS);
            res.set_content(jsonRes.dump(), "application/json");
        }

        return success;
    }

    template<typename T, typename... Types>
    void handle(httplib::Response &res, T &handler, Types... args) {
        ERROR_CODE err{ERROR_CODE::OK};
        const auto response = handler(err, args...);

        nlh::json jsonRes;
        if (err != ERROR_CODE::OK) {
            jsonRes["error"] = static_cast<int>(err);
        } else {
            jsonRes["response"] = response;
        }
        res.set_content(jsonRes.dump(), "application/json");
    }
}// namespace

namespace Server {
    HttpServer::HttpServer() {
        svr.Get("/exit", [this](const httplib::Request &, httplib::Response &res) {
            res.set_content("OK", "text/plain");
            svr.stop();
        });
    }

    void HttpServer::listen(int port) {
        std::cout << "Listen on port: " << port << std::endl;
        svr.listen("0.0.0.0", port);
    }

    void HttpServer::setAuthHandler(std::function<nlh::json(ERROR_CODE &err, std::string, int)> handler) {
        svr.Post("/auth", [handler = std::move(handler)](const httplib::Request &req, httplib::Response &res) {
            auto jsonReq = nlh::json::parse(req.body);
            if (!validateArgs(res, jsonReq, {"id", "version"})) {
                return;
            }

            handle(res, handler, jsonReq["id"], jsonReq["version"]);
        });
    }

    void HttpServer::setRegisterHandler(std::function<nlh::json(ERROR_CODE &err, std::string, std::string, int)> handler) {
        svr.Post("/register", [handler = std::move(handler)](const httplib::Request &req, httplib::Response &res) {
            auto jsonReq = nlh::json::parse(req.body);
            if (!validateArgs(res, jsonReq, {"id", "name", "version"})) {
                return;
            }

            handle(res, handler, jsonReq["id"], jsonReq["name"], jsonReq["version"]);
        });
    }

    void HttpServer::setSendHandler(std::function<nlh::json(ERROR_CODE &err, std::string, std::string)> handler) {
        svr.Post("/send", [handler = std::move(handler)](const httplib::Request &req, httplib::Response &res) {
            auto jsonReq = nlh::json::parse(req.body);
            if (!validateArgs(res, jsonReq, {"id", "instruction"})) {
                return;
            }

            handle(res, handler, jsonReq["id"], jsonReq["instruction"]);
        });
    }

    void HttpServer::setTuneHandler(std::function<nlh::json(ERROR_CODE &err, std::string)> handler) {
        svr.Post("/tune", [handler = std::move(handler)](const httplib::Request &req, httplib::Response &res) {
            auto jsonReq = nlh::json::parse(req.body);
            if (!validateArgs(res, jsonReq, {"id"})) {
                return;
            }

            handle(res, handler, jsonReq["id"]);
        });
    }

    void HttpServer::setBoostHandler(std::function<nlh::json(ERROR_CODE &err, std::string)> handler) {
        svr.Post("/boost", [handler = std::move(handler)](const httplib::Request &req, httplib::Response &res) {
            auto jsonReq = nlh::json::parse(req.body);
            if (!validateArgs(res, jsonReq, {"id"})) {
                return;
            }

            handle(res, handler, jsonReq["id"]);
        });
    }
}// namespace Server
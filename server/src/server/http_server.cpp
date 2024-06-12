#include "server/http_server.h"
#include "ext/json.h"

#include <optional>

namespace {
    std::optional<nlh::json> validateArgs(const httplib::Request &req, httplib::Response &res, const std::vector<std::string> &names) {
        bool success = true;
        nlh::json jsonReq;
        try {
            jsonReq = nlh::json::parse(req.body);
            for (const auto &name: names) {
                success &= jsonReq.contains(name);
            }
        } catch (...) {
            success = false;
        }

        if (!success) {
            nlh::json jsonRes;
            jsonRes["error"] = static_cast<int>(ERROR_CODE::WRONG_REQUEST_ARGS);
            res.set_content(jsonRes.dump(), "application/json");
            return std::nullopt;
        }

        return jsonReq;
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
            if (auto jsonReq = validateArgs(req, res, {"id", "version"}); jsonReq.has_value()) {
                auto &json = *jsonReq;
                handle(res, handler, json["id"], json["version"]);
            }
        });
    }

    void HttpServer::setRegisterHandler(std::function<nlh::json(ERROR_CODE &err, std::string, std::string, int)> handler) {
        svr.Post("/register", [handler = std::move(handler)](const httplib::Request &req, httplib::Response &res) {
            if (auto jsonReq = validateArgs(req, res, {"id", "name", "version"}); jsonReq.has_value()) {
                auto &json = *jsonReq;
                handle(res, handler, json["id"], json["name"], json["version"]);
            }
        });
    }

    void HttpServer::setSendHandler(std::function<nlh::json(ERROR_CODE &err, std::string, std::string)> handler) {
        svr.Post("/send", [handler = std::move(handler)](const httplib::Request &req, httplib::Response &res) {
            if (auto jsonReq = validateArgs(req, res, {"id", "instruction"}); jsonReq.has_value()) {
                auto &json = *jsonReq;
                handle(res, handler, json["id"], json["instruction"]);
            }
        });
    }

    void HttpServer::setTuneHandler(std::function<nlh::json(ERROR_CODE &err, std::string)> handler) {
        svr.Post("/tune", [handler = std::move(handler)](const httplib::Request &req, httplib::Response &res) {
            if (auto jsonReq = validateArgs(req, res, {"id"}); jsonReq.has_value()) {
                auto &json = *jsonReq;
                handle(res, handler, json["id"]);
            }
        });
    }

    void HttpServer::setBoostHandler(std::function<nlh::json(ERROR_CODE &err, std::string)> handler) {
        svr.Post("/boost", [handler = std::move(handler)](const httplib::Request &req, httplib::Response &res) {
            if (auto jsonReq = validateArgs(req, res, {"id"}); jsonReq.has_value()) {
                auto &json = *jsonReq;
                handle(res, handler, json["id"]);
            }
        });
    }
}// namespace Server
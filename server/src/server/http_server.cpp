#include "server/http_server.h"
#include "ext/json.h"

namespace Server {
    HttpServer::HttpServer() {

        svr.Get("/auth", [](const httplib::Request &, httplib::Response &res) {
            res.set_content("Hello World", "text/plain");
        });

        svr.Get("/tune", [](const httplib::Request &, httplib::Response &res) {
            res.set_content("Hello World", "text/plain");
        });
        svr.Get("/boost", [](const httplib::Request &, httplib::Response &res) {
            res.set_content("Hello World", "text/plain");
        });
    }

    void HttpServer::listen(int port) {
        std::cout << "Listen on port: " << port << std::endl;
        svr.listen("0.0.0.0", port);
    }

    void HttpServer::setRegisterHandler(std::function<bool(std::string /*name*/)> handler) {
        svr.Post("/register", [handler = std::move(handler)](const httplib::Request &req, httplib::Response &res) {
            nlh::json jsonReq = nlh::json::parse(req.body);
            nlh::json jsonRes;

            bool success = true;
            success &= jsonReq.contains("name");
            if (!success) {
                jsonRes["error"] = "Wrong args";
                res.set_content(jsonRes.dump(), "application/json");
                return;
            }
            std::string name = jsonReq["name"];

            const auto answer = handler(name);
            jsonRes["answer"] = answer;
            res.set_content(jsonRes.dump(), "application/json");
        });
    }

    void HttpServer::setSendHandler(std::function<std::string(std::string_view, std::string_view)> handler) {
        svr.Post("/send", [handler = std::move(handler)](const httplib::Request &req, httplib::Response &res) {
            nlh::json jsonReq = nlh::json::parse(req.body);
            nlh::json jsonRes;

            bool success = true;
            success &= jsonReq.contains("usr");
            success &= jsonReq.contains("instruction");

            if (!success) {
                jsonRes["error"] = "Wrong args";
                res.set_content(jsonRes.dump(), "application/json");
                return;
            }

            const std::string usr = jsonReq["usr"];
            const std::string instruction = jsonReq["instruction"];

            std::cout << "Send ARGS: " << std::endl
                      << "USR: " << usr << std::endl
                      << "Instruction: " << instruction << std::endl;

            const auto answer = handler(usr, instruction);
            std::cout << "Response to client: " << answer << std::endl;

            jsonRes["answer"] = answer;
            res.set_content(jsonRes.dump(), "application/json");
        });
    }
}// namespace Server
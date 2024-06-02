#include "ai_client/ai.h"
#include "logic/logic.h"
#include "server/http_server.h"
#include "utils/config.h"

#include <iostream>

ERROR_CODE validateVersion(int version) {
    if (version == Utils::Config::instance().getVersion()) {
        return ERROR_CODE::OK;
    }

    return ERROR_CODE::WRONG_CLIENT_VERSION;
}

int main(int argc, char *argv[]) {
    std::srand(std::time(nullptr));
    try {
        assert(argc == 2);
        if (argc != 2) {
            std::cout << "Wrong args: expected call ./server config.json" << std::endl;
            return 0;
        }

        Utils::Config::instance().load(argv[1]);
        Utils::Config::instance().setVersion(2);

        Logic::CoreLogic logic;
        Server::HttpServer server;
        server.setAuthHandler([&logic](ERROR_CODE &err, const std::string &id, int version) {
            nlh::json res;
            err = validateVersion(version);
            if (err == ERROR_CODE::OK) {
                StrategyType type;
                err = logic.authUser(id, type);
                res["strategy"] = static_cast<int>(type);
            }
            return res;
        });
        server.setRegisterHandler([&logic](ERROR_CODE &err, const std::string &id, const std::string &name, int version) {
            nlh::json res;
            err = validateVersion(version);
            if (err == ERROR_CODE::OK) {
                err = logic.registerUser(id, name);
            }
            if (err == ERROR_CODE::OK) {
                StrategyType type;
                err = logic.tune(id, type);
                res["strategy"] = static_cast<int>(type);
            }

            return res;
        });
        server.setSendHandler([&logic](ERROR_CODE &err, const std::string &id, const std::string &instruction) {
            std::string response;
            err = logic.send(id, instruction, response);
            return response;
        });
        server.setTuneHandler([&logic](ERROR_CODE &err, const std::string &id) {
            nlh::json res;
            StrategyType type;
            err = logic.tune(id, type);
            res["strategy"] = static_cast<int>(type);
            return res;
        });
        server.setBoostHandler([&logic](ERROR_CODE &err, const std::string &id) {
            err = logic.boost(id);
            return nlh::json();
        });

        server.listen(Utils::Config::instance().getPort());
        logic.save();
        Utils::Config::instance().save();
    } catch (...) {
        std::cerr << "something went wrong";
    }

    return 0;
}

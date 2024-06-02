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

        Utils::Config::instance().load(argv[1]);

        Logic::CoreLogic logic;
        Server::HttpServer server;
        server.setAuthHandler([&logic](ERROR_CODE &err, std::string_view name, int version) {
            err = validateVersion(version);
            if (err == ERROR_CODE::OK) {
                err = logic.authUser(name);
            }
            return nlh::json();
        });
        server.setRegisterHandler([&logic](ERROR_CODE &err, std::string_view name, int version) {
            err = validateVersion(version);
            if (err == ERROR_CODE::OK) {
                err = logic.registerUser(name);
            }
            return nlh::json();
        });
        server.setSendHandler([&logic](ERROR_CODE &err, std::string_view name, std::string_view instruction) {
            std::string response;
            err = logic.send(name, instruction, response);
            return response;
        });
        server.setTuneHandler([&logic](ERROR_CODE &err, std::string_view name) {
            err = logic.tune(name);
            return nlh::json();
        });
        server.setBoostHandler([&logic](ERROR_CODE &err, std::string_view name) {
            err = logic.boost(name);
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

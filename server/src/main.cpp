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
        server.setAuthHandler([&logic](ERROR_CODE &err, std::string_view id, int version) {
            err = validateVersion(version);
            if (err == ERROR_CODE::OK) {
                err = logic.authUser(std::string{id});
            }
            return nlh::json();
        });
        server.setRegisterHandler([&logic](ERROR_CODE &err, std::string_view id, int version) {
            err = validateVersion(version);
            if (err == ERROR_CODE::OK) {
                err = logic.registerUser(std::string{id});
            }
            return nlh::json();
        });
        server.setSendHandler([&logic](ERROR_CODE &err, std::string_view id, std::string_view instruction) {
            std::string response;
            err = logic.send(std::string{id}, instruction, response);
            return response;
        });
        server.setTuneHandler([&logic](ERROR_CODE &err, std::string_view id) {
            err = logic.tune(std::string{id});
            return nlh::json();
        });
        server.setBoostHandler([&logic](ERROR_CODE &err, std::string_view id) {
            err = logic.boost(std::string{id});
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

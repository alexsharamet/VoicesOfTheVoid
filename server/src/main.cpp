#include "ai_client/ai.h"
#include "logic/logic.h"
#include "server/http_server.h"
#include "utils/config.h"

#include <iostream>

int main(int argc, char *argv[]) {
    try {
        assert(argc == 2);

        Utils::Config::instance().load(argv[1]);

        Logic::CoreLogic logic;
        Server::HttpServer server;
        server.setAuthHandler([&logic](std::string_view name) {
            return logic.authUser(name);
        });
        server.setRegisterHandler([&logic](std::string_view name) {
            return logic.registerUser(name);
        });
        server.setSendHandler([&logic](std::string_view usr, std::string_view instruction) {
            return logic.send(usr, instruction);
        });

        server.listen(Utils::Config::instance().getPort());
        logic.save();
        Utils::Config::instance().save();
    } catch (...) {
        std::cerr << "something went wrong";
    }

    return 0;
}

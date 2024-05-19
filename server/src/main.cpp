#include "ai_client/ai.h"
#include "logic/logic.h"
#include "server/http_server.h"

#include <iostream>

nlh::json readConfig() {
    nlh::json cnfg;
    cnfg["ai_host"] = "192.168.1.68";
    cnfg["ai_port"] = 5001;

    nlh::json alex;
    alex["name"] = "alex";

    nlh::json users;
    users.push_back(alex);

    cnfg["users"] = users;

    return cnfg;
}

int main() {
    try {
        auto cnfg = readConfig();
        std::cout << "Read config :" << cnfg << std::endl;

        Logic::CoreLogic logic(cnfg);
        Server::HttpServer server;
        server.setRegisterHandler([&logic](std::string_view name) {
            return logic.registerUser(name);
        });
        server.setSendHandler([&logic](std::string_view usr, std::string_view instruction) {
            return logic.send(usr, instruction);
        });

        server.listen(8080);

        cnfg = logic.toJson();
        std::cout << "Write config :" << cnfg << std::endl;
    } catch (...) {
        std::cerr << "something went wrong";
    }

    return 0;
}

cmake_minimum_required(VERSION 3.22)
project(server)

set(CMAKE_CXX_STANDARD 17)
set(SHARED_FOLDER ${CMAKE_SOURCE_DIR}/../shared)

set(HEADERS
        ${SHARED_FOLDER}/error_codes.h
        ${SHARED_FOLDER}/strategies.h
        inc/ext/json.h
        inc/ai_client/ai.h
        inc/server/http_server.h
        inc/logic/promt.h
        inc/logic/logic.h
        inc/logic/user.h
        inc/logic/strategy.h
        inc/logic/strategy/ai_strategy.h
        inc/logic/strategy/echo_strategy.h
        inc/logic/strategy/core_corrupt_stategy.h
        inc/utils/config.h
        inc/utils/random.h)

set(SOURCE
        src/ai_client/ai.cpp
        src/server/http_server.cpp
        src/logic/logic.cpp
        src/logic/user.cpp
        src/logic/strategy/ai_strategy.cpp
        src/logic/strategy/echo_strategy.cpp
        src/logic/strategy/core_corrupt_strategy.cpp
        src/utils/config.cpp
        src/utils/random.cpp
        src/main.cpp)

add_executable(${PROJECT_NAME}
        ${HEADERS}
        ${SOURCE})

target_include_directories(${PROJECT_NAME}
        PRIVATE ${CMAKE_SOURCE_DIR}/inc
        PRIVATE ${SHARED_FOLDER})

find_package(nlohmann_json REQUIRED)
find_package(httplib REQUIRED)
find_package(fmt REQUIRED)

target_link_libraries(${PROJECT_NAME}
        PRIVATE nlohmann_json::nlohmann_json
        PRIVATE httplib::httplib
        PRIVATE fmt::fmt)

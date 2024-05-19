cmake_minimum_required(VERSION 3.22)
project(server)

set(CMAKE_CXX_STANDARD 17)

set(HEADERS
        inc/ai_client/ai.h)

set(SOURCE
        src/ai_client/ai.cpp
        src/main.cpp)

add_executable(${PROJECT_NAME}
        ${HEADERS}
        ${SOURCE})

target_include_directories(${PROJECT_NAME}
        PRIVATE ${CMAKE_SOURCE_DIR}/inc)

find_package(nlohmann_json REQUIRED)
find_package(CURL REQUIRED)

target_link_libraries(${PROJECT_NAME}
        PRIVATE CURL::libcurl
        PRIVATE nlohmann_json::nlohmann_json)


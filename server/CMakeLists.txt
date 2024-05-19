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

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/inc)
target_link_libraries(${PROJECT_NAME} CURL)


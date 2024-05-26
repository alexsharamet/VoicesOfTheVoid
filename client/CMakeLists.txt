cmake_minimum_required(VERSION 3.22)
project(client)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(SHARED_FOLDER ${CMAKE_SOURCE_DIR}/../shared)

find_package(Qt6Core REQUIRED)
find_package(Qt6Quick REQUIRED)
find_package(Qt6Gui REQUIRED)
find_package(Qt6Qml REQUIRED)

set(HEADERS
        ${SHARED_FOLDER}/error_codes.h
        inc/ui/mainPageViewModel.h
        inc/ui/mainPageModel.h
        inc/utils/config.h
        inc/logic/network.h
        )

set(SOURCE
        src/ui/mainPageViewModel.cpp
        src/ui/mainPageModel.cpp
        src/utils/config.cpp
        src/logic/network.cpp
        src/main.cpp)

set(RESOURCES
        res/ui/form.qml
        res/ui/back.jpeg
        )

add_executable(${PROJECT_NAME}
        ${HEADERS}
        ${SOURCE})

target_include_directories(${PROJECT_NAME}
        PRIVATE ${CMAKE_SOURCE_DIR}/inc
        PRIVATE ${SHARED_FOLDER})

target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt6::Core Qt6::Quick Qt6::Gui Qt6::Qml)

qt_add_resources(${PROJECT_NAME} "client"
        PREFIX "/"
        FILES ${RESOURCES}
        )

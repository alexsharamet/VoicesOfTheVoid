cmake_minimum_required(VERSION 3.22)
project(client)

include(QtLoader.cmake)
set(MAIN_PROJECT_NAME VoicesOfTheVoid)

set(CMAKE_CXX_STANDARD 17)
LoadQtMacros()
set(SHARED_FOLDER ${CMAKE_SOURCE_DIR}/../shared)

findQt()

set(HEADERS
        ${SHARED_FOLDER}/error_codes.h
        ${SHARED_FOLDER}/strategies.h
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

add_executable(${PROJECT_NAME} MACOSX_BUNDLE
        ${HEADERS}
        ${SOURCE})

set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
        )

target_include_directories(${PROJECT_NAME}
        PRIVATE ${CMAKE_SOURCE_DIR}/inc
        PRIVATE ${SHARED_FOLDER})

target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt6::Core Qt6::Quick Qt6::Gui Qt6::Qml Qt6::DBus)

qt_add_resources(${PROJECT_NAME} "client"
        PREFIX "/"
        FILES ${RESOURCES}
        )

qt_add_qml_module(${PROJECT_NAME}
        URI gui
        VERSION 2.0
        QML_FILES res/ui/form.qml)

set(BUNDLE_NAME "${MAIN_EXE_NAME}.app")
set(BUNDLE_DIR "${CMAKE_CURRENT_BINARY_DIR}/bin/${BUNDLE_NAME}")
set(BUNDLE_CONTENTS_DIR "${BUNDLE_DIR}/Contents")
set(BUNDLE_PLIST "${BUNDLE_CONTENTS_DIR}/Info.plist")
set(BUNDLE_FRAMEWORKS_DIR "${BUNDLE_CONTENTS_DIR}/Frameworks")
set(BUNDLE_RESOURCES "${CMAKE_SOURCE_DIR}/packages/mac")
set(PREPARE_BUNDLE_DIR "${CMAKE_BINARY_DIR}/_bundle")

add_custom_target(prepare_deploy DEPENDS ${PROJECT_NAME})
findQtDeploy("prepare_deploy" OFF)

if (APPLE)
    set(PREPARE_PKG_DIR "${CMAKE_BINARY_DIR}/_pkg")
    configure_file("${BUNDLE_RESOURCES}/Info.plist.in" "${PREPARE_BUNDLE_DIR}/Info.plist" @ONLY)
    configure_file("${BUNDLE_RESOURCES}/pkg-component.plist.in" "${PREPARE_PKG_DIR}/component.plist" @ONLY)
    configure_file("${BUNDLE_RESOURCES}/pkg-distribution.xml.in" "${PREPARE_PKG_DIR}/distribution.xml" @ONLY)
    configure_file("${BUNDLE_RESOURCES}/pkg-postinstall.in" "${PREPARE_PKG_DIR}/scripts/postinstall" @ONLY)

    message(STATUS "copy ${BUNDLE_RESOURCES}/PkgInfo")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy "${BUNDLE_RESOURCES}/PkgInfo" ${BUNDLE_CONTENTS_DIR})

    add_custom_target(final_bundle DEPENDS prepare_deploy)

    message(STATUS "delete existing ${MAIN_BUNDLE_DIR}")
    add_custom_command(TARGET final_bundle POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E rm -rf "${MAIN_BUNDLE_DIR}")

    #copyOpenSSLLibs(TARGET ${PROJECT_NAME} DIR "${BUNDLE_CONTENTS_DIR}/MacOS/" LIBS ssl crypto)

    # rename bundle
    message(STATUS "[rename] Bundle will be renamed from [${PROJECT_NAME}] to [${MAIN_PROJECT_NAME}]")
    add_custom_command(TARGET final_bundle POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E rename "${BUNDLE_CONTENTS_DIR}/MacOS/${MAIN_EXE_NAME}" "${BUNDLE_CONTENTS_DIR}/MacOS/${MAIN_PROJECT_NAME}"
            COMMAND ${CMAKE_COMMAND} -E rename "${BUNDLE_DIR}" "${MAIN_BUNDLE_DIR}")

    message(STATUS "-> copy ${PREPARE_BUNDLE_DIR}/ru.lproj to ${MAIN_BUNDLE_DIR}/Contents/Resources")
    add_custom_command(TARGET final_bundle POST_BUILD
            COMMAND cp -rf "${PREPARE_BUNDLE_DIR}/ru.lproj" "${MAIN_BUNDLE_DIR}/Contents/Resources")

    message(STATUS "-> copy ${PREPARE_BUNDLE_DIR}/en.lproj to ${MAIN_BUNDLE_DIR}/Contents/Resources")
    add_custom_command(TARGET final_bundle POST_BUILD
            COMMAND cp -rf "${PREPARE_BUNDLE_DIR}/en.lproj" "${MAIN_BUNDLE_DIR}/Contents/Resources")

    message(STATUS "-> copy ${PREPARE_BUNDLE_DIR}/Info.plist to ${MAIN_BUNDLE_PLIST}")
    add_custom_command(TARGET final_bundle POST_BUILD
            COMMAND cp -f "${PREPARE_BUNDLE_DIR}/Info.plist" "${MAIN_BUNDLE_PLIST}")

    message(STATUS "-> plist Set :LSMinimumSystemVersion ${CMAKE_OSX_DEPLOYMENT_TARGET}")
    add_custom_command(TARGET final_bundle POST_BUILD
            COMMAND ${PLIST_BUDDY} -c "Set :LSMinimumSystemVersion ${CMAKE_OSX_DEPLOYMENT_TARGET}" ${MAIN_BUNDLE_PLIST} VERBATIM)

    message(STATUS "-> plist Set :CFBundleVersion ${BUILD_NUMBER}")
    add_custom_command(TARGET final_bundle POST_BUILD
            COMMAND ${PLIST_BUDDY} -c "Set :CFBundleVersion ${BUILD_NUMBER}" ${MAIN_BUNDLE_PLIST} VERBATIM)

    message(STATUS "-> plist Set :CFBundleShortVersionString ${MAIN_VERSION_STRING}")
    add_custom_command(TARGET final_bundle POST_BUILD
            COMMAND ${PLIST_BUDDY} -c "Set :CFBundleShortVersionString ${MAIN_VERSION_STRING}" ${MAIN_BUNDLE_PLIST} VERBATIM)
elseif (MSVC)
    add_custom_command(TARGET prepare_deploy PRE_BUILD
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} .)
    #copyOpenSSLLibs(TARGET ${PROJECT_NAME} DIR "${CMAKE_CURRENT_BINARY_DIR}/bin" LIBS ssl crypto)
endif ()

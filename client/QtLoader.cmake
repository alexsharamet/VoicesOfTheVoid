macro(LoadQtMacros)
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
endmacro()

macro(findQt)
    set(QT_PACKAGES
            Core
            Gui
            Quick
            Qml
            DBus)

    find_package(Qt6 COMPONENTS ${QT_PACKAGES} REQUIRED)
    message("Current Qt6 version is ${Qt6_VERSION}")

    if (APPLE)
        set(QT_MAC_EXTRAS Qt6::MacExtras)
    elseif(MSVC)
        set(QT_WIN_EXTRAS Qt6::WinExtras)
    endif()
endmacro()

macro(findQtDeploy TARGET_NAME)
    find_program(QT6_DEPLOY_CMD NAMES windeployqt.exe macdeployqt)

    if(NOT QT6_DEPLOY_CMD)
        if (APPLE OR MSVC)
            message(FATAL_ERROR "Qt deployment tool not found! ${Qt6_Core_LIB_DIRS}/../bin")
        endif()
    else()
        message(STATUS "Qt deployment tool: ${QT6_DEPLOY_CMD}")
        set(QT6_DEPLOY_TYPE "--debug")

        if(CMAKE_BUILD_TYPE)
            if(${CMAKE_BUILD_TYPE} STREQUAL "Release" OR ${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
                set(QT6_DEPLOY_TYPE "--release")
            endif()
        endif()

        if(MSVC)
            set(QT6_DEPLOY_CMD_ARGS ${QT6_DEPLOY_TYPE} --angle --no-compiler-runtime --translations en,ru --verbose 2 .)
            if (PRESERVE_DEBUG_INFO)
                set(QT6_DEPLOY_CMD_ARGS ${QT6_DEPLOY_CMD_ARGS} --pdb)
            endif()
            set(QT6_DEPLOY_TARGET_DIR ${CMAKE_CURRENT_BINARY_DIR}/bin)
        elseif(APPLE)
            set(QT6_DEPLOY_CMD_ARGS . -verbose=2)
            if (PRESERVE_DEBUG_INFO)
                set(QT6_DEPLOY_CMD_ARGS ${QT6_DEPLOY_CMD_ARGS} -no-strip)
            endif()
            set(QT6_DEPLOY_TARGET_DIR ${BUNDLE_DIR})
        else()
            set(QT6_DEPLOY_CMD_ARGS .)
            set(QT6_DEPLOY_TARGET_DIR ${CMAKE_CURRENT_BINARY_DIR}/bin)
        endif()

        add_custom_command(TARGET ${TARGET_NAME} PRE_BUILD
                WORKING_DIRECTORY ${QT6_DEPLOY_TARGET_DIR}
                COMMAND ${QT6_DEPLOY_CMD} ${QT6_DEPLOY_CMD_ARGS})
    endif()
endmacro()

cmake_minimum_required(VERSION 3.10)
project(webserv)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include
include_directories(${CMAKE_SOURCE_DIR}/includes)
include_directories(${CMAKE_SOURCE_DIR}/srcs)
include_directories(${CMAKE_SOURCE_DIR}/damianServer)


# srcs
file(GLOB MAIN_SRC "${CMAKE_SOURCE_DIR}/srcs/main.cpp")
file(GLOB RESPONSE_SRCS "${CMAKE_SOURCE_DIR}/srcs/Response/*.cpp")
set(SOURCES ${MAIN_SRC} ${RESPONSE_SRCS}
        Config/backup/ServerConfig.cpp
        Config/backup/ServerConfig.hpp
        Config/ConfigValidator.cpp
        Config/ConfigValidator.hpp
        Config/ConfigParser.cpp
        Config/ConfigParser.hpp
        Config/MimeType.cpp
        Config/MimeType.hpp
        Config/backup/Location.cpp
        Config/backup/Location.hpp
        Config/test/test_main.cpp
        Config/backup/ServerConfigFallbacks.hpp
        Config/backup/ServerConfigFallbacks.cpp
        Config/backup/ServerConfigBase.cpp
        Config/backup/ServerConfigBase.hpp
        Config/test/test_parser_module.cpp
        Config/ConfigUnit.cpp
        Config/ConfigUnit.hpp
        Config/Location.cpp
        Config/Location.hpp
        Config/ServerConfig.cpp
        Config/ServerConfig.hpp
        Config/ConfigCommon.hpp
)

add_executable(webserv ${SOURCES})

target_include_directories(webserv PRIVATE
        ${CMAKE_SOURCE_DIR}/includes
        ${CMAKE_SOURCE_DIR}/srcs
)
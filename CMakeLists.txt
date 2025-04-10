cmake_minimum_required(VERSION 3.10)
project(webserv)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include
include_directories(${CMAKE_SOURCE_DIR}/includes)
include_directories(${CMAKE_SOURCE_DIR}/srcs)

# srcs
file(GLOB MAIN_SRC "${CMAKE_SOURCE_DIR}/srcs/main.cpp")
file(GLOB RESPONSE_SRCS "${CMAKE_SOURCE_DIR}/srcs/Response/*.cpp")
set(SOURCES ${MAIN_SRC} ${RESPONSE_SRCS}
        backup/Request.hpp
        includes/StatusQueue.hpp
        srcs/Response/StatusQueue.cpp
        includes/StatusEntry.hpp)

add_executable(webserv ${SOURCES})

target_include_directories(webserv PRIVATE
        ${CMAKE_SOURCE_DIR}/includes
        ${CMAKE_SOURCE_DIR}/srcs
)
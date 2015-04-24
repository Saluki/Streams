cmake_minimum_required(VERSION 3.1)
project(Streams)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.c)
add_executable(Streams ${SOURCE_FILES} server.h server.c constants.h log.h log.c lock.h lock.c)
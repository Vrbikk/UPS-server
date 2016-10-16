cmake_minimum_required(VERSION 3.6)
project(server2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(SOURCE_FILES src/main.cpp src/Game.cpp src/Game.h src/Client.cpp src/Client.h src/Connection.cpp src/Connection.h)
add_executable(server2 ${SOURCE_FILES})
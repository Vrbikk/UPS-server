cmake_minimum_required(VERSION 3.0.2)
project(server2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(SOURCE_FILES src/main.cpp src/Game.cpp src/Game.h src/ClientCommunication.cpp src/ClientCommunication.h src/Logger.cpp src/Logger.h src/protocol.cpp src/protocol.h src/Server.cpp src/Server.h src/tools.cpp src/tools.h src/Configuration.cpp src/Configuration.h src/GameLogic.cpp src/GameLogic.h)
add_executable(server2 ${SOURCE_FILES})
//
// Created by vrbik on 19.10.16.
//

#ifndef SERVER2_SERVER_H
#define SERVER2_SERVER_H

#include <thread>
#include <memory>
#include "Game.h"

class Server {
private:
    std::shared_ptr<Connection> connection;
    std::shared_ptr<Game> game;

    std::thread server_thread;
    bool server_running;
    void serverRunner();
public:
    Server();
    ~Server();
    bool initServer();
};


#endif //SERVER2_SERVER_H

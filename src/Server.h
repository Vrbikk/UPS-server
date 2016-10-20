//
// Created by vrbik on 19.10.16.
//

#ifndef SERVER2_SERVER_H
#define SERVER2_SERVER_H

#include <thread>
#include <memory>
#include "Game.h"
#include "Configuration.h"

class Server {
private:
    std::shared_ptr<Connection> connection;
    std::shared_ptr<Game> game;
    std::shared_ptr<Logger> logger;

    server_config server_conf;
    std::thread server_thread;
    bool server_running;
    void serverRunner();
public:
    Server(server_config server_config_);
    ~Server();
    bool initServer();
};


#endif //SERVER2_SERVER_H

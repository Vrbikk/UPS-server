//
// Created by vrbik on 19.10.16.
//

#include "Server.h"

Server::Server(server_config server_config_) : server_conf(server_config_) {}
Server::~Server() {

    server_running = false;

    if(server_thread.joinable()){
        server_thread.join();
    }
}

void Server::serverRunner() {

    while(server_running) {
        //herní smyčka bude tady snad
        game->startGame();
    }

}

bool Server::initServer() {
    connection = std::make_shared<Connection>();
    if(connection->initConnection(server_conf.port)){
        game = std::make_shared<Game>(connection); //tadyyyy posílam shared pointer
        connection->initAccepting(game);
        server_running = true;
        server_thread = std::thread(&Server::serverRunner, this);
        return true;
    }else{
        return false;
    }
}

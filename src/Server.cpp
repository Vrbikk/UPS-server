//
// Created by vrbik on 19.10.16.
//

#include "Server.h"

Server::Server() {}
Server::~Server() {

    server_running = false;

    if(server_thread.joinable()){
        server_thread.join();
    }
}

void Server::serverRunner() {

    while(server_running) {
        game.startGame();
    }

}

bool Server::initServer() {
    connection = std::make_shared<Connection>();

    if(connection->initConnection()){
        Game game(connection); //tadyyyy posílam shared pointer
        connection.initAccepting(&game);
        server_running = true;
        server_thread = std::thread(&Server::serverRunner, this);
        return true;
    }else{
        return false;
    }
}

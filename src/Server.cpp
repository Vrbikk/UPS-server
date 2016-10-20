//
// Created by vrbik on 19.10.16.
//

#include "Server.h"

Server::Server(server_config server_config_) : server_conf(server_config_) {
    logger = std::make_shared<Logger>(server_conf.logging_file, false);

    logger->Info("--- " + server_conf.get_server_name() + " started ---");
    logger->Info(server_conf.get_server_params());
}

Server::~Server() {
    logger->Info("server is shutting down");

    server_running = false;

    if(server_thread.joinable()){
        server_thread.join();
    }
}

void Server::serverRunner() {
    logger->Info("server Thread is running");
    while(server_running) {
        game->startGame();
    }
}

bool Server::initServer() {
    connection = std::make_shared<Connection>(logger);
    if(connection->initConnection(server_conf.port)){
        game = std::make_shared<Game>(connection, logger);
        connection->initAccepting(game);
        server_running = true;
        server_thread = std::thread(&Server::serverRunner, this);
        return true;
    }else{
        return false;
    }
}

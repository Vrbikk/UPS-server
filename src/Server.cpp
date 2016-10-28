//
// Created by vrbik on 19.10.16.
//

#include "Server.h"

Server::Server(server_config server_config_) : server_conf(server_config_) {
    logger = std::make_shared<Logger>(server_conf.logging_file, true, server_conf.enable_logging);
    logger->Info("--- " + server_conf.get_server_name() + " started ---");
    logger->Info(server_conf.get_server_params());
}

Server::~Server() {

    logger->Info("server is shutting down");

    accepting_running = false;

    shutdown(server_socket, SHUT_RDWR);
    close(server_socket);

    if(accepting_thread.joinable()){
        accepting_thread.join();
    }
}

bool Server::initServer() {
    if(initConnection(server_conf.port)){
        game = std::make_shared<Game>(logger, server_conf.number_of_clients);
        initAccepting();
        return true;
    }else{
        return false;
    }
}

bool Server::initConnection(int port) {
    struct sockaddr_in serv_addr, client_address;

    server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (server_socket < 0) {
        logger->Error("Socket could not be initialized");
        return false;
    }

    logger->Info("Initializing socket OK");

    memset(&serv_addr, 0, sizeof(sockaddr_in));
    memset(&client_address, 0, sizeof(sockaddr_in));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY; // binding socket_number to all interfaces
    serv_addr.sin_port = htons(port); // transfers port into 16bit number format

    int optval = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    int binding_status = bind(server_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

    if(binding_status < 0){
        logger->Error("Socket binding failed");
        return false;
    }

    logger->Info("Binding socket OK");

    int listen_status = listen(server_socket, tcp_queue);

    if(listen_status != 0){
        logger->Error("Listening on socket failed");
        return false;
    }

    logger->Info("Listening on socket OK");
    return true;
}

void Server::acceptingRunner() {

    while(accepting_running) {

        struct sockaddr_in client_address;
        socklen_t size = sizeof(client_address);

        int connection_id = 0;

        connection_id = accept(server_socket, (struct sockaddr *) &client_address, &size);

        if(connection_id > 0){
            std::unique_ptr<ClientCommunication> client_communication = std::unique_ptr<ClientCommunication>(new ClientCommunication(
                    connection_id, client_address, game, logger));
            game->Attach(std::move(client_communication));
        }
    }
}

void Server::initAccepting() {
    accepting_running = true;
    accepting_thread = std::thread(&Server::acceptingRunner, this);
}

std::string Server::getStatus() {
    std::string status = server_conf.get_server_name() + " max clients:" + std::to_string(server_conf.number_of_clients) + "\n";
    status += game->gameStatus();

    return status;
}

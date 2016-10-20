//
// Created by vrbik on 16.10.16.
//

#include "Connection.h"

bool Connection::initConnection(int port) {
    struct sockaddr_in serv_addr, client_address;

    // initializing TCP socket_number
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

void Connection::acceptingRunner() {
    while(accepting_running) {

        struct sockaddr_in client_address;
        socklen_t size = sizeof(client_address);

        int connection_id = accept(server_socket, (struct sockaddr *) &client_address, &size);

        std::unique_ptr<Client> client = std::unique_ptr<Client>(new Client(
                connection_id, client_address, game, logger));
        game->Attach(std::move(client));
    }
}

void Connection::initAccepting(std::shared_ptr<Game> game_) {
    accepting_running = true;
    game = game_;
    accepting_thread = std::thread(&Connection::acceptingRunner, this);
}

Connection::~Connection() {
    accepting_running = false;

    if(accepting_thread.joinable()){
        accepting_thread.join();
    }
}

Connection::Connection(std::shared_ptr<Logger> logger_) : logger(logger_) {}



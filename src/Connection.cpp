//
// Created by vrbik on 16.10.16.
//

#include "Connection.h"

bool Connection::initConnection() {
    struct sockaddr_in serv_addr, client_address;

    // initializing TCP socket_number
    server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (server_socket < 0) {
        LOGGER->Error("Socket could not be initialized");
        return false;
    }

    LOGGER->Info("Initializing socket OK");

    memset(&serv_addr, 0, sizeof(sockaddr_in));
    memset(&client_address, 0, sizeof(sockaddr_in));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY; // binding socket_number to all interfaces
    serv_addr.sin_port = htons(server_port); // transfers port into 16bit number format

    int optval = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    int binding_status = bind(server_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

    if(binding_status < 0){
        LOGGER->Error("Socket binding failed");
        return false;
    }

    LOGGER->Info("Binding socket OK");

    int listen_status = listen(server_socket, tcp_queue);

    if(listen_status != 0){
        LOGGER->Error("Listening on socket failed");
        return false;
    }

    LOGGER->Info("Listening on socket OK");
    return true;
}

void Connection::acceptingRunner() {
    while(accepting_running) {

        struct sockaddr_in client_address;
        socklen_t size = sizeof(client_address);

        int connection_id = accept(server_socket, (struct sockaddr *) &client_address, &size);

        std::this_thread::sleep_for (std::chrono::seconds(1));
        std::unique_ptr<Client> client = std::unique_ptr<Client>(new Client(
                connection_id, client_address, game));
        game->Attach(std::move(client));
    }
}

void Connection::initAccepting(Game *_game) {
    accepting_running = true;
    game = _game;
    accepting_thread = std::thread(&Connection::acceptingRunner, this);
}

Connection::~Connection() {
    accepting_running = false;

    if(accepting_thread.joinable()){
        accepting_thread.join();
    }
}



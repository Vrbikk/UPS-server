//
// Created by vrbik on 16.10.16.
//

#include "Client.h"

void Client::clientRunner(){

    char input[buffer_size];
    while(client_running){
        memset(input, 0, (size_t)buffer_size);
        int result = (int)read(connection_id, &input, (size_t)(buffer_size - 1));

        if(result <= 0){
            sending_status = false;
            clientDisconnected();
        }else{
            handleInput(std::string(input));
        }
    }
}

Client::Client(int connection_id, sockaddr_in address_ , std::shared_ptr<Game> game_, std::shared_ptr<Logger> logger_) :
        connection_id(connection_id), address(address_), game(game_), logger(logger_) {}

void Client::initThread() {
    client_running = true;
    client_thread = std::thread(&Client::clientRunner, this);
}

Client::~Client() {
    client_running = false;
    sending_status = false;
    closeConnection();

    if(client_thread.joinable()){
        client_thread.join();
    }
}

void Client::clientDisconnected() {
    logger->Error("DISCONNECTED " + getStatus());
    game->addIndexToGarbage(id);
    game->wakeupGarbageCollector();
    std::this_thread::sleep_for (std::chrono::milliseconds(100));
}

void Client::handleInput(std::string input) {
    if(is_valid_message(input)){
        message msg = decompose_message(input, id);
        game->resolveMessage(msg);
    }else{
        logger->Error("BAD message! : " + std::string(input));
        sendMessage(compose_message(ERROR, "invalid message"));
    }
}

void Client::sendMessage(message msg) {
    if(sending_status) {
        std::string a = msg.raw;
        a += '\n';
        send(connection_id, a.c_str(), a.size(), 0);
    }else{
        logger->Error("Could not send message, client is not running id:" + std::to_string(id));
    }
}

void Client::closeConnection() {
    close(connection_id);
}

std::string Client::getStatus() {
    return std::string("Client id:[" + std::to_string(id) + "] name:[" + data.name + "] logged:[" + std::to_string(data.logged)
    + "] ready:[" + std::to_string(data.ready) + "]");
}

//
// Created by vrbik on 16.10.16.
//

#include "ClientCommunication.h"

void ClientCommunication::clientRunner(){

    char input[buffer_size];
    while(listener_running){
        memset(input, 0, (size_t)buffer_size);
        int result = (int)read(socket_number, &input, (size_t)(buffer_size - 1));

        if(result <= 0){
            sending_permission = false;
            listener_running = false;
            clientDisconnected();
        }else{
            handleInput(std::string(input));
        }
    }
}

ClientCommunication::ClientCommunication(int connection_id, sockaddr_in address_ ,
                                         std::shared_ptr<Game> game_, std::shared_ptr<Logger> logger_) :
        socket_number(connection_id), address(address_), game(game_), logger(logger_) {}

void ClientCommunication::initThread() {
    listener_running = true;
    client_thread = std::thread(&ClientCommunication::clientRunner, this);
}

ClientCommunication::~ClientCommunication() {
    listener_running = false;
    sending_permission = false;
    closeConnection();

    if(client_thread.joinable()){
        client_thread.join();
    }
}

void ClientCommunication::clientDisconnected() {
    logger->Error("DISCONNECTED " + getStatus());
    game->addToGarbage(id);
    game->wakeupGarbageCollector();
    std::this_thread::sleep_for (std::chrono::milliseconds(100));
}

void ClientCommunication::handleInput(std::string input) {
    if(is_valid_message(input)){
        event e = make_event(EVENT_message, decompose_message(input), id);
        game->resolveEvent(e);
    }else{
        logger->Error("BAD message! : " + std::string(input));
        sendMessage(compose_message(ERROR, "detected invalid message"));
    }
}

void ClientCommunication::sendMessage(message msg) {

    if(sending_permission) {
        std::string a = msg.raw;
        a += '\n';
        send(socket_number, a.c_str(), a.size(), 0);
    }else{
        logger->Error("CC could not send message:" + getStatus());
    }
}

void ClientCommunication::closeConnection() {
    close(socket_number);
}

std::string ClientCommunication::getStatus() {
    return std::string("CC status id:[" + id.print() + "]" + " socket_number:[" + std::to_string(socket_number)
                       + "] listener_running:[" + std::to_string(listener_running) + "]");
}

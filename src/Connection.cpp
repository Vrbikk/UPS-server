//
// Created by vrbik on 16.10.16.
//

#include "Connection.h"

bool Connection::initConnection() {
    return true;
}

bool running;

void Connection::clientAccepting_Thread() {
    while(game->activeClients < game->maxClients){
        std::cout << "CONN: adding client" << std::endl;
        Client client(game, 1, 1);
        game->Attach(&client);
    }

    //std::cout << "joining" << std::endl;

    game->joinClients(); //TODO pruser
}

void Connection::setUpAccepting(Game *_game) {
    game = _game;
    std::thread acceptor(&Connection::clientAccepting_Thread, this);
    acceptor.join();
}


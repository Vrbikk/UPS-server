//
// Created by vrbik on 15.10.16.
//

#ifndef SERVER2_GAME_H
#define SERVER2_GAME_H


#include "Client.h"
#include <vector>
#include <algorithm>

class Client;

class Game {
    std::vector<Client*> clientList;
public:
    int activeClients = 0;
    int maxClients = 3;
    void Attach(Client *client);
    void Detach(Client *client);
    void Notify(int number);
    void joinClients();
};



#endif //SERVER2_GAME_H

//
// Created by vrbik on 15.10.16.
//

#ifndef SERVER2_GAME_H
#define SERVER2_GAME_H


#include "Client.h"
#include <memory>
#include <vector>
#include <algorithm>

class Client;

class Game {
private:
    std::vector<std::unique_ptr<Client>> clientList = std::vector<std::unique_ptr<Client>>(3);
public:
    int maxClients = 3;
    int activeClients = 0;
    void Attach(std::unique_ptr<Client> client);
    void Detach(int client_id);
    void Notify(int number);
    void initClients();
    int getFreeIndex();
    void clientDisconnected(std::unique_ptr<Client> client);
};



#endif //SERVER2_GAME_H

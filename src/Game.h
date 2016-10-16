//
// Created by vrbik on 15.10.16.
//

#ifndef SERVER2_GAME_H
#define SERVER2_GAME_H


#include <vector>
#include <algorithm>

class Client;

class Game {
    std::vector<Client*> clientList;
public:
    void Attach(Client *client);
    void Detach(Client *client);
    void Notify(int number);
};



#endif //SERVER2_GAME_H

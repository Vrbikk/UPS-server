//
// Created by vrbik on 15.10.16.
//

#ifndef SERVER2_GAME_H
#define SERVER2_GAME_H

#include "ClientCommunication.h"
#include "Logger.h"
#include <thread>
#include <memory>
#include <vector>
#include <algorithm>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "GameLogic.h"
#include "Configuration.h"

class ClientCommunication;
class GameLogic;

struct client{
    bool empty = true;
    bool ready = false;
    bool logged = false;
    bool online = false;
    std::string name = "";
    int score = 0;
    std::unique_ptr<ClientCommunication> communication;

    std::string print(){
        return std::string("CLIENT - name:[" + name +
                                   "] r:[" + std::to_string(ready) +
                                   "] l:[" + std::to_string(logged) +
                                   "] o:[" + std::to_string(online) + "]");
    }

    void reset(){
        empty = true;
        ready = false;
        logged = false;
        online = false;
        name = "";
        score = 0;
    }
};

class Game{
private:

    std::vector<std::unique_ptr<client>> clients;

    std::unique_ptr<GameLogic> gameLogic;

    std::vector<std::unique_ptr<ClientCommunication>> pendingClients;

    std::queue<client_id> garbageQueue;

    std::thread garbage_collector_thread;
    std::mutex mutex_garbage_collector;
    std::mutex mutex_add_index;
    std::mutex resolve_mutex;
    std::condition_variable cv;
    bool garbage_collector_running = false;
    bool garbage_ready;
    std::shared_ptr<Logger> logger;

public:
    Game(std::shared_ptr<Logger> logger_, int number_of_clients, std::vector<question> questions);
    virtual ~Game();

    //STATS
    int maxClients = 0;
    int activeClients = 0;
    void incrementActiveClients();
    void decrementActiveClients(client_id id);

    //OBSERVERS MANIPULATION
    void Attach(std::unique_ptr<ClientCommunication> client_communication);
    void Detach(client_id id);

    //GARBAGE COLLECTOR
    void addToGarbage(client_id id);
    void garbageCollectorThread();
    void initGarbageCollector();
    void wakeupGarbageCollector();

    void resolveEvent(event e);

    //LOGING
    void login(event e);
    int getFreePendingPosition();
    int getFreeActivePosition();
    int getLoggedPosition(std::string name);
    bool isDuplicatedLogin(std::string name);
    void renewClient(unsigned long logged_index, event e);
    void loginNewClient(unsigned long new_index, event e);
    void moveLoggedClient(unsigned long pending_index, unsigned long active_index);

    //SENDING
    void sendMessageToPendingClient(unsigned long pending_index, message msg);
    void sendMessageToClient(unsigned long index, message msg);
    void sendMessageToAllClients(message msg);
    std::string getQuestionsData(std::vector<question> questions);

    //READY
    bool isClientReady(unsigned long index);
    void ready(unsigned long index);
    bool isEveryoneReady();

    void cleaningClients();
    void clearScore();
    std::string getClientName(unsigned long index);
    unsigned long getNextPlayerIndex(int index);
    unsigned long nextPossibleClientIndex(int index);
    void increasePoints(unsigned long index, int points);
    void gameResult();

    //TOOLS
    std::string readyList();
    void info();
    std::string clientInfo(unsigned long index);
    std::string gameStatus();

};



#endif //SERVER2_GAME_H

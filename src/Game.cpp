//
// Created by vrbik on 15.10.16.
//

#include "Game.h"

void Game::Attach(std::unique_ptr<ClientCommunication> client_communication) {

    int pending_index = getFreePendingPosition();
    if(pending_index >= 0){
        logger->Info("Adding client to pending: with id:" + std::to_string(pending_index));
        client_communication->id.index = pending_index;
        pendingClients.at((unsigned long)pending_index) = std::move(client_communication);
        pendingClients.at((unsigned long)pending_index)->initThread();
    }else{
        logger->Error("Could not add more clients to pending.");
        client_communication->sendMessage(compose_message(ERROR, "Server queue is full!"));
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        //now the client_communication destructor is called
    }
}

void Game::Detach(client_id id) {

    if(id.pending && pendingClients.at(id.index) != nullptr){
        logger->Info("Removing pending client: " + std::to_string(id.index));
        pendingClients.at(id.index).reset(nullptr);
    }
    else if(!id.pending && clients.at(id.index)->communication != nullptr){

        clients.at(id.index)->online = false;
        clients.at(id.index)->ready = false;

        logger->Info("Removing active client: " + clients.at(id.index)->print());
        clients.at(id.index)->communication.reset(nullptr);

        sendToAllClients(compose_message(BROADCAST, clients.at(id.index)->name + " has disconnected"));

        decrementActiveClients();

    }else{
        logger->Info("could not remove client");
    }
}

int Game::getFreePendingPosition() {
    int index = 0;
    for(auto&& client: pendingClients){
         if(!client){
             return index;
        }
        index++;
    }
    return -1;
}

void Game::garbageCollectorThread() {
    while(garbage_collector_running){
        std::unique_lock<std::mutex> lk(mutex_garbage_collector);
        cv.wait(lk, [&]{return garbage_ready;});

        while(!garbageQueue.empty()) {
            Detach(garbageQueue.front());
            garbageQueue.pop();
        }

        garbage_ready = false;
        lk.unlock();
    }
}

void Game::initGarbageCollector() {
    garbage_ready = false;
    garbage_collector_running = true;
    garbage_collector_thread = std::thread(&Game::garbageCollectorThread, this);
}

void Game::wakeupGarbageCollector() {
    std::lock_guard<std::mutex> lk(mutex_garbage_collector);
    garbage_ready = true;
    cv.notify_one();
}

void Game::addToGarbage(client_id id) {
    std::lock_guard<std::mutex> lk(mutex_add_index);
    garbageQueue.push(id);
}

Game::Game(std::shared_ptr<Logger> logger_, int number_of_clients_, std::vector<question> questions) :
        logger(logger_), maxClients(number_of_clients_){
    initGarbageCollector();
    gameLogic = std::unique_ptr<GameLogic>(new GameLogic(this, logger, questions));
    clients = std::vector<std::unique_ptr<client>>((unsigned long)maxClients);

    for(int i = 0; i < maxClients; i++){
        clients.at(i) = std::move(std::unique_ptr<client>(new client));
    }

    pendingClients = std::vector<std::unique_ptr<ClientCommunication>>((unsigned long)maxClients);
}

Game::~Game() {
    garbage_collector_running = false;
    wakeupGarbageCollector();

    if(garbage_collector_thread.joinable()){
        garbage_collector_thread.join();
    }
}

void Game::resolveEvent(event e) {
    std::lock_guard<std::mutex> lk(resolve_mutex);

    switch(e.e_type){
        case EVENT_message:{
            if(e.msg.m_type == LOGIN_C && e.id.pending){
                login(e);
            }else if(!e.id.pending){
                gameLogic->input(e); //passing events to the game
            }
            break;
        }
        case EVENT_client_disconnected:{
            gameLogic->input(e); //special events for client disconnection
            break;
        }
        default:{
            logger->Error("Unexpected event type");
            break;
        }
    }
}

void Game::sendMessageToClient(unsigned long index, message msg) {
    if(clients.at(index)->online){
        clients.at(index)->communication->sendMessage(msg);
    }else{
        logger->Error("Attempt to send message to offline client");
    }
}

std::string Game::gameStatus() {
    std::string game_status = "";

    for(auto &&a: pendingClients){
        if(a == nullptr){
            game_status += "----EMPTY----\n";
        }else{
            game_status += a->getStatus() + "\n";
        }
    }

    return  game_status;
}

void Game::sendToAllClients(message msg) {
    for(auto &&client : clients){
        if(client->online){
            client->communication->sendMessage(msg);
        }
    }
}

std::string Game::readyList() {
    std::string list = "---ready_list---";

    for(auto &&client : clients){
        if(client->online && client->ready){
            list += "/(" + client->name + ") - ready";
        }else{
            if(client->logged) {
                list += "/(" + client->name + ") - not ready";
            }else{
                list += "/[waiting] - not ready";
            }
        }
    }

    return list;
}

bool Game::isClientReady(unsigned long index) {
    return clients.at(index)->ready;
}

void Game::ready(unsigned long index) {
    if(clients.at(index)->online && clients.at(index)->logged && !clients.at(index)->ready){
        clients.at(index)->ready = true;
        sendMessageToClient(index, compose_message(READY_S, "ok"));
    }else{
        logger->Error("Could not set ready to the client: " + clients.at(index)->print());
    }
}

bool Game::isEveryoneReady() {
    for(auto &&client: clients){
        if(!client->ready){
            return false;
        }
    }
    return true;
}

int Game::getFreeActivePosition() {
    int index = 0;
    for(auto&& client: clients){
        if(client->empty){
            return index;
        }
        index++;
    }
    return -1;
}

int Game::getLoggedPosition(std::string name) {
    int index = 0;
    for(auto&& client: clients){
        if(!client->empty && !client->online && client->name.compare(name) == 0){
            return index;
        }
        index++;
    }
    return -1;
}

bool Game::isDuplicatedLogin(std::string name) {
    for(auto&& client: clients){
        if(!client->empty && client->online && client->name.compare(name) == 0){
            return true;
        }
    }
    return false;
}

void Game::renewClient(unsigned long logged_index, event e) {
    logger->Info("Renewing client with name: " + clients.at(logged_index)->name);

    moveLoggedClient(e.id.index, logged_index);

    clients.at(logged_index)->online = true;

    sendMessageToClient(logged_index, compose_message(UNICAST_S, "welcome back!"));
    sendMessageToClient(logged_index, compose_message(LOGIN_S, (int)logged_index));

    incrementActiveClients();
}

void Game::loginNewClient(unsigned long new_index, event e) {

    logger->Info("Logging new client with name: " + e.msg.data);

    moveLoggedClient(e.id.index, new_index);

    clients.at(new_index)->empty = false;
    clients.at(new_index)->online = true;
    clients.at(new_index)->logged = true;
    clients.at(new_index)->name = e.msg.data;

    sendMessageToClient(new_index, compose_message(UNICAST_S, "Welcome on the server!"));
    sendMessageToClient(new_index, compose_message(LOGIN_S, new_index));

    incrementActiveClients();
}

void Game::login(event e) {
    int logged_index = getLoggedPosition(e.msg.data);

    if(logged_index >= 0){
        renewClient((unsigned long)logged_index, e);
    }
    else if(!isDuplicatedLogin(e.msg.data)){
        int new_index = getFreeActivePosition();
        if(new_index >= 0){
            loginNewClient((unsigned long)new_index, e);
        }else{
            logger->Error("Bad login attempt, Server is full, login name: " + e.msg.data);
            sendMessageToPendingClient(e.id.index, compose_message(UNICAST_S, "Server is full!"));
        }
    }else{
        logger->Error("Bad login attempt, Server already have player: " + e.msg.data);
        sendMessageToPendingClient(e.id.index, compose_message(UNICAST_S, "Name already used!"));
    }

}

void Game::moveLoggedClient(unsigned long pending_index, unsigned long active_index) {
    clients.at(active_index)->communication = std::move(pendingClients.at(pending_index));
    clients.at(active_index)->communication->id.index = active_index;
    clients.at(active_index)->communication->id.pending = false;
}

void Game::sendMessageToPendingClient(unsigned long pending_index, message msg) {
    pendingClients.at(pending_index)->sendMessage(msg);
}

void Game::incrementActiveClients() {
    activeClients++;
    logger->Info("Active Clients changed: " + std::to_string(activeClients) + "/" + std::to_string(maxClients));
}

void Game::decrementActiveClients() {
    activeClients--;
    logger->Info("Active Clients changed: " + std::to_string(activeClients) + "/" + std::to_string(maxClients));

    if(activeClients == 0){
        cleaningClients();
    }else if(activeClients >= 1){
        resolveEvent(disconnection_event());
    }

}

std::string Game::clientInfo(unsigned long index) {
    return clients.at(index)->print();
}

void Game::info() {
    for(auto &&client : clients){
        logger->Info("info:" + client->print());
    }
}

void Game::cleaningClients() {

    logger->Info("0 active clients, cleaning clients");

    for(auto &&client : clients){
        client->reset();
    }
}

void Game::sendQuestionsToAllClients(std::vector<question> questions) {
    std::string msg_text = "";
    for(auto q : questions){
        msg_text += std::to_string(q.question_id) + "_" +
                std::to_string(q.points) + "_" + std::to_string(q.avaible) + "-";
    }

    sendToAllClients(compose_message(QUESTIONS_S, msg_text.substr(0, msg_text.size() - 1)));
}

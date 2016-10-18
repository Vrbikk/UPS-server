#include "Game.h"

int main() {

    LOGGER->setUp("log.log");

    LOGGER->Info("Server started.");

    Connection connection;

    if(connection.initConnection()){
        Game game(&connection);
        connection.initAccepting(&game);

        game.startGame();

        std::this_thread::sleep_for(std::chrono::seconds(8));



        game.addIndexToGarbage(0);
        game.addIndexToGarbage(1);

        game.wakeupGarbageCollector();

        std::this_thread::sleep_for (std::chrono::seconds(3));

        //game.Notify(200);*/

        while(true){
           // game.Notify(200);
           // std::this_thread::sleep_for (std::chrono::seconds(1));
        }
    }

    return 0;
}
#include "Game.h"
#include "Connection.h"

int main() {

    Connection connection;

    if(connection.initConnection()){
        Game game;
        connection.setUpAccepting(&game);
        connection.wakeupRunner();

        std::this_thread::sleep_for (std::chrono::seconds(5));
        game.Detach(0);
        game.Detach(2);
        std::this_thread::sleep_for (std::chrono::seconds(5));

        connection.wakeupRunner();

        std::this_thread::sleep_for (std::chrono::seconds(5));
        game.Detach(1);
        std::this_thread::sleep_for (std::chrono::seconds(5));

        connection.wakeupRunner();

        while(true);
    }



    return 0;
}
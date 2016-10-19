#include "Game.h"

int main(int argc, char *argv[]) {

    LOGGER->setUp("log.log");

    if(argc == 2 && CONFIG->setUp(argv[1])){
        LOGGER->Info("Server started.");

        Configuration::getConfiguration()->setUp(argv[1]);

        Connection connection;

        if(connection.initConnection()){
            Game game(&connection);
            connection.initAccepting(&game);

            game.startGame();

            while(true){
                // game.Notify(200);
                std::this_thread::sleep_for (std::chrono::seconds(10));
            }
        }

    }else{
        LOGGER->Error("Config file not found, ending");
    }

    return 0;
}
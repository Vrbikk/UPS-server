#include "Game.h"

int main() {

    Connection connection;

    if(connection.initConnection()){
        Game game;
        connection.setUpAccepting(&game);
        game.initGame(&connection);
        while(true);
    }

    return 0;
}
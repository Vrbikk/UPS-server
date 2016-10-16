#include "Game.h"
#include "Connection.h"
int main() {
    /*
    Game game;

    Client a(&game);
    Client b(&game);
    Client c(&game);

    game.Attach(&a);
    game.Attach(&b);
    game.Attach(&c);

    game.Notify(10);

    a.sendToAll(100);

    game.Detach(&a);

    game.Notify(20);*/

    Connection connection;

    if(connection.initConnection()){
        Game game;
        connection.setUpAccepting(&game);
    }



    return 0;
}
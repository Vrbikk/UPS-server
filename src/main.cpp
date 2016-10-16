#include "Game.h"
#include "Client.h"
int main() {

    Game game;

    Client a(&game);
    Client b(&game);
    Client c(&game);

    game.Attach(&a);
    game.Attach(&b);
    game.Attach(&c);

    game.Notify(10);

    a.sendToAll(100);


    return 0;
}
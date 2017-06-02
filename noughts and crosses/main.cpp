#include <iostream>
#include "Game.h"

#define PLAY_GAME true

using namespace std;

int main()
{
    #if PLAY_GAME
    cout << "type NUMBER, NUBMER ENTER" << endl;
    cout << "for example 1, 3 ENTER" << endl;
    Game game;
    cout << game.getBoard() << endl;
    while(!game.hasEnded()){
        game.nextPlayer();
        cout << "it's player " << game.getCurrentPlayerChar() << "'s turn" << endl;
        game.getMove();
        cout << game.getBoard() << endl;
        //cout << boolalpha << game.hasEnded() << endl;
    }
    if(game.isFull()){
        cout << "game has ended in a tie" << endl;
    }else{
        cout << "player " << game.getCurrentPlayerChar() << " has won" << endl;
    }
    #else
    cout << "DEBUG mode" << endl;
    Game game;
    #endif
    return 0;
}



#ifndef GAME_H
#define GAME_H


#include "Board.h"



class Game
{
    Board board;
    player currPlayer;
    bool hasWon(player currentPlayer);
    bool AI_player_1 = false;
    bool AI_player_2 = true;

public:
    Game();
    virtual ~Game();

    const Board& getBoard() const
    {
        return board;
    }

    Board& getBoard()
    {
        return board;
    }

    //int AI_getTileScore(position pos, player currPlayer);

    bool hasWon();

    bool isFull();

    bool hasEnded();

    char getCurrentPlayerChar(){
        return currPlayer.getPiece();
    }

    void nextPlayer(){
        currPlayer = currPlayer.nextPlayer();
    }

    void placePiece(position pos)
    {
        board.placePiece(pos, currPlayer);
    }

    pair<bool, position> AI_getFinish();

    void getMove();

    bool isInputCorrect(int x, int y);
    bool isInputCorrect(position p);

protected:

private:
};

#endif // GAME_H

#include "Game.h"
#include <iostream>
#include <sstream>
#include <string>

Game::Game()
{
/*
    //board.placePiece(position(2, 0), currPlayer);
    //board.placePiece(position(2, 1), currPlayer);
    //currPlayer = currPlayer.nextPlayer();
    //board.placePiece(position(2, 2), currPlayer);
    //board.placePiece(position(0, 0), currPlayer);
    //board.placePiece(position(2, 0), currPlayer);
    board.placePiece(position(0, 2), currPlayer);
    board.placePiece(position(2, 2), currPlayer);
    //board.placePiece(position(1, 1), currPlayer);
    //board.placePiece(position(2, 1), currPlayer);
    currPlayer = currPlayer.nextPlayer();
    //board.placePiece(position(0, 0), currPlayer);
    board.placePiece(position(2, 1), currPlayer);
    //board.placePiece(position(1,1), currPlayer);
    //board.placePiece(position(2,2), currPlayer);
    board.placePiece(position(0,1), currPlayer);
    //board.placePiece(position(2,0), currPlayer);
    board.placePiece(position(1,0), currPlayer);
    //board.placePiece(position(0,1), currPlayer);
    //board.placePiece(position(1,2), currPlayer);
    //currPlayer = currPlayer.nextPlayer();
    //nextPlayer();

    cout << board << endl;
    //cout << boolalpha << hasWon()  << endl;
    board.printTileScoresBoard(currPlayer);
    //cout << board.getTileScore(currPlayer, currPlayer, position(1,2), false);
    //cout << board.getTileScore(currPlayer, currPlayer, position(0,0), true);
   // cout << board.isWinningMove(currPlayer, position(1,0), true);
*/
}


Game::~Game()
{
    //dtor
}

pair<bool, position> Game::AI_getFinish()
{
    for(int x = 0; x < 3; x++) {
        for(int y = 0; y < 3; y++) {
            goto end_loop;
        }
    }
end_loop:
    return std::make_pair(true,position(1,1));
}


bool Game::hasWon(player currentPlayer)
{

    auto pieces = board.getPieces();
    int count_cross_forward = 0; /*       "/"      */
    int count_cross_backward = 0; /*      "\"      */
    for(int i = 0; i < 3; i++) {
        int count_vertical = 0;
        int count_horizontal = 0;
        for(int j = 0; j < 3; j++) {
            if(pieces[i][j] == currentPlayer.getPiece()) {
                count_vertical++;
            }
            if(pieces[j][i] == currentPlayer.getPiece()) {
                count_horizontal++;
            }
        }

        if(pieces[i][i] == currentPlayer.getPiece()) {
            count_cross_backward++;
        }
        if(pieces[2 - i][i] == currentPlayer.getPiece()) {
            count_cross_forward++;
        }

        if (count_vertical == 3 || count_horizontal == 3 ) return true;
    }
    if(count_cross_backward == 3 || count_cross_forward == 3) return true;
    return false;
}

bool Game::hasWon()
{
    //cout << "evaluating has won of "  << currPlayer.getPiece() << endl;
    bool won = hasWon(currPlayer);
    //cout << "the result is " << boolalpha << won << endl;
    return won;
}


bool Game::isFull()
{
    return board.isFull();
}

bool Game::hasEnded()
{
    return isFull() || hasWon();
}

bool Game::isInputCorrect(int x, int y)
{
    if(board.getPieces()[x][y] != Board::empty_char) return false;

    return x >= 0 && x < 3 && y >= 0 && y < 3;
}

bool Game::isInputCorrect(position p)
{
    return isInputCorrect(p.x, p.y);
}

void Game::getMove()
{
    if((AI_player_1 && currPlayer.number == 1) || (AI_player_2 && currPlayer.number == 2)) {
        position bestMove = board.getBestMove(currPlayer);
        cout << "AI moved at " <<  bestMove.x + 1 << " " << bestMove.y + 1<< endl;
        //board.printTileScoresBoard(currPlayer);
        placePiece(bestMove);
    }else{
        int x, y;
        string input;

        while(1) {
            std::getline (std::cin, input);
            std::size_t found = input.find(",");
            if(found != string::npos) {
                input.replace(found, 1, " ");
            }
            //cout << "input " << input << endl;
            stringstream ss(input);
            if(ss >> x >> y && isInputCorrect(x-1, y-1)) {
                break;
            }
            cout << "input not correct" << endl;
            cin.clear();
        }
        placePiece(position(x-1, y-1));
    }
}

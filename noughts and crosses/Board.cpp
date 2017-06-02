#include "Board.h"
#include <iostream>
#include <string>

char Board::empty_char = ' ';
char Board::player_1_char = 'X';
char Board::player_2_char = 'O';


char player::getPiece() const
{
    return number == 1? Board::player_1_char : Board::player_2_char;
}

Board::Board()
{
    //ctor
    for(auto &i: pieces) {
        i.fill(empty_char);
    }
}

Board::~Board()
{
    //dtor
}



bool Board::isFull()
{
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if(pieces[i][j] == Board::empty_char) {
                return false;
            }
        }
    }
    return true;
}


ostream& operator<< (ostream& os, const Board& b)
{
    auto pieces = b.getPieces();
    std::string spacing = "     ";
    std::string horizontalLine = "  --  --  --";
    os << spacing << horizontalLine << endl;

    for(int y = 0; y < 3; y++) {
        os << spacing << "| ";
        for(int x = 0; x < 3; x++) {
            os << pieces[x][y] << " | ";
        }
        os << endl << spacing << horizontalLine << endl;
    }
    return os;
}

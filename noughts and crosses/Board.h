#ifndef BOARD_H
#define BOARD_H
#include "array"
#include <assert.h>
#include <iostream>


using namespace std;

class Board;

template<bool...> struct bool_pack;
template<bool... bs>
using all_true = std::is_same<bool_pack<bs..., true>, bool_pack<true, bs...>>;

struct position {
    int x;
    int y;
    position(int _x, int _y)
    {
        assert ( _x >= 0 && _x < 3);
        assert ( _y >= 0 && _y < 3);
        x = _x;
        y = _y;
    };
    /*position()
    {
        x = 0;
        y = 0;
    };*/
};


struct player {
    int number;
    player(const int _number)
    {
        assert(_number == 1 || _number == 2);
        number = _number;
    }
    player nextPlayer()
    {
        if(number == 1) {
            return player(2);
        } else {
            return player(1);
        }
    }
    player()
    {
        number = 2;
    }
    char getPiece() const;
};



typedef array< array <char, 3>, 3 > pieces_t;

class Board
{
    pieces_t pieces;
public:

    const pieces_t& getPieces() const
    {
        return pieces;
    }

    void setPiece(position p, char v)
    {
        pieces[p.x][p.y] = v;
    }

    void setAllPieces(pieces_t newPieces)
    {
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++) {
                pieces[i][j] = newPieces[i][j];
            }
        }
    }


    bool isFull();

// 1 = win
// -1 = loss
// 0 = tie

    int getMaxScore(const player startingPlayer, player currPlayer)
    {
        int maxScore = -10; // score can't be lower than -2
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++) {
                maxScore = max (getTileScore(startingPlayer, currPlayer, position(i,j)), maxScore);
            }
        }
        //cout << "max score " << maxScore << endl;
        return maxScore;
    }

    int getMinScore(const player startingPlayer, player currPlayer)
    {
        int minScore = 10; // score can't be higher than 1
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++) {
                int score = getTileScore(startingPlayer, currPlayer, position(i,j));
                //cout << score << endl;
                if(score >= -1) {
                    minScore = min (score, minScore);
                }
            }
        }
        //cout << "min score " << minScore << endl;
        return minScore;
    }

    void printTileScoresBoard(player currPlayer, bool debug = false)
    {
        cout << "tile scores of player " << currPlayer.getPiece() << endl;
        Board scoreBoard;
        for(int x = 0; x < 3; x++) {
            for(int y = 0; y < 3; y++) {
                int score = getTileScore(currPlayer, currPlayer, position(x, y), debug);
                char score_c;
                if(score == 0) {
                    score_c = '0';
                } else {
                    if(score == 1) {
                        score_c = '1';
                    } else {
                        if(score == -1) {
                            score_c = '-';
                        } else {
                            if(score == -2) {
                                score_c = 'U';
                            } else {
                                cout << "invalid score of " << score << endl;
                                score_c = 'N';
                            }
                        }
                    }
                }
                scoreBoard.setPiece(position(x, y),score_c);
            }
        }
        cout << scoreBoard << endl;
    }

    position getBestMove(player currPlayer)
    {
        //cout << "evaluating move of " << currPlayer.getPiece() << endl;
        position bestPosition(0,0);


        int highestScore = -10;
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++) {
                int score = getTileScore(currPlayer, currPlayer, position(i,j));
                if(score > highestScore) {
                    highestScore = score;
                    bestPosition = position(i, j);
                    //cout << "better move " <<  bestPosition.x << " " << bestPosition.y << endl;
                }
            }
        }
        return bestPosition;
    }


    int getTileScore(const player startingPlayer, player currPlayer, position tilePosition, bool debug = false)
    {
        //cout << currPlayer.getPiece() << endl;
        if(pieces[tilePosition.x][tilePosition.y] != Board::empty_char) {
            return -2;
        }
        if(isWinningMove(currPlayer, tilePosition, debug)) {
            if(currPlayer.number == startingPlayer.number) {
                return 1;
            } else {
                return -1;
            }
        }
        Board newBoard;
        newBoard.setAllPieces(this->getPieces());
        newBoard.setPiece(tilePosition, currPlayer.getPiece());
        if(newBoard.isFull()) {
                //cout << "board is full " << endl;
                return 0;
        }
        currPlayer = currPlayer.nextPlayer();
        if(currPlayer.number == startingPlayer.number) {
            return newBoard.getMaxScore(startingPlayer, currPlayer);
        } else {
            return newBoard.getMinScore(startingPlayer, currPlayer);
        }
    }

    bool isWinningMove(player currPlayer, position pos, bool debug = false)
    {
        // assume tile isn't empty
        if(horizontalCount(currPlayer, pos.y) == 2 ) {
            if(debug) cout << "horizontal" << endl;
            return true;
        }
        if(verticalCount(currPlayer, pos.x) == 2 ) {
            if(debug) cout << "vertical" << endl;
            return true;
        }
        if(pos.x == pos.y && crossBackwardCount(currPlayer) == 2) {
            if(debug) cout << "backward" << endl;
            return true;
        }
        if(2-pos.x == pos.y && crossForwardCount(currPlayer) == 2) {
            if(debug) cout << "forward" << endl;
            return true;
            //return 2-pos.x == pos.y;
        }
        return false;
    }


    void winPrint(player winningPlayer, string winText, position winningPosition)
    {
        cout << "winning player ( " << winningPlayer.getPiece() << " ) " << endl;
        cout << "the winning move is " << winningPosition.x + 1 << ", " << winningPosition.y + 1 << endl;
        cout << winText << endl;
    }


    pair<bool, position> canPlayWinning(player currPlayer, bool printWin = false)
    {
        bool _canPlayWinning = false;
        position winningPosition = position(0, 0);


        if(crossForwardCount(currPlayer) == 2 && crossForwardCount(currPlayer.nextPlayer()) == 0) {
            _canPlayWinning = true;
            int emptySpot = getCrossForwardEmptySpot();
            winningPosition = position(2-emptySpot, emptySpot);

            if(printWin) winPrint(currPlayer, "cross forward", winningPosition);
            // return the things
            return std::make_pair(_canPlayWinning, winningPosition);
        }

        if(crossBackwardCount(currPlayer) == 2 && crossBackwardCount(currPlayer.nextPlayer()) == 0) {
            _canPlayWinning = true;
            int emptySpot = getCrossBackwardEmptySpot();
            winningPosition = position(emptySpot, emptySpot);

            if(printWin) winPrint(currPlayer, "cross backward ", winningPosition);
            // return the things
            return std::make_pair(_canPlayWinning, winningPosition);
        }

        for(int i = 0 ; i<3; i++) {

            // horizontal
            if(horizontalCount(currPlayer, i) == 2 && horizontalCount(currPlayer.nextPlayer(), i) == 0) {
                _canPlayWinning = true;
                winningPosition = position(getHorizontalEmptySpot(i), i);

                if(printWin) winPrint(currPlayer, "horizontal", winningPosition);
                // return the things
                return std::make_pair(_canPlayWinning, winningPosition);
            }

            // vertical
            if(verticalCount(currPlayer, i) == 2 && verticalCount(currPlayer.nextPlayer(), i) == 0) {
                _canPlayWinning = true;
                winningPosition = position(i, getVerticalEmptySpot(i));

                if(printWin) winPrint(currPlayer, "vertical", winningPosition);
                return std::make_pair(_canPlayWinning, winningPosition);
            }
        }
        if(printWin) cout << "player ( " << currPlayer.getPiece() << " ) can't win"<< endl;

        return std::make_pair(false, winningPosition);
    }

    int getCrossBackwardEmptySpot()
    {
        for(int i = 0; i < 3; i++) {
            if(pieces[i][i] == Board::empty_char) {
                return i;
            }
        }
        assert(false);
        return -1;
    }

    int getCrossForwardEmptySpot()
    {
        for(int i = 0; i < 3; i++) {
            if(pieces[2-i][i] == Board::empty_char) {
                return i;
            }
        }
        assert(false);
        return -1;
    }

    int getHorizontalEmptySpot(int y)
    {
        for(int x = 0; x < 3; x++) {
            if(pieces[x][y] == Board::empty_char) {
                return x;
            }
        }
        assert(false);
        return -1;
    }


    int getVerticalEmptySpot(int x)
    {
        for(int y = 0; y < 3; y++) {
            if(pieces[x][y] == Board::empty_char) {
                return y;
            }
        }
        assert(false);
        return -1;
    }

    int horizontalCount(player currPlayer, int posY)
    {
        int count_horizontal = 0;
        for(int j = 0; j < 3; j++) {
            if(pieces[j][posY] == currPlayer.getPiece()) {
                count_horizontal++;
            }
        }
        return count_horizontal;
    }

    int verticalCount(player currPlayer, int posX)
    {
        int count_vertical = 0;
        for(int j = 0; j < 3; j++) {
            if(pieces[posX][j] == currPlayer.getPiece()) {
                count_vertical++;
            }
        }
        return count_vertical;
    }

    int crossForwardCount(player currPlayer) // forward cross is /  (forward slash)
    {
        int count_crossForward = 0;
        for(int j = 0; j < 3; j++) {
            if(pieces[j][2-j] == currPlayer.getPiece()) {
                count_crossForward++;
            }
        }
        return count_crossForward;
    }

    int crossBackwardCount(player currPlayer)  /* backward cross is \  (backslash) */
    {
        int count_crossBackward = 0;
        for(int j = 0; j < 3; j++) {
            if(pieces[j][j] == currPlayer.getPiece()) {
                count_crossBackward++;
            }
        }
        return count_crossBackward;
    }


    void placePiece(position pos, player p)
    {
        pieces[pos.x][pos.y] = p.getPiece();
    }
    static char empty_char;
    static char player_1_char;
    static char player_2_char;

    friend ostream& operator << (ostream& os, const Board& b);

    Board();
    virtual ~Board();


protected:

private:
};

#endif // BOARD_H

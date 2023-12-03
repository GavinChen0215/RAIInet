#ifndef __BOARD__H__
#define __BOARD__H__

#include <vector>
#include "Link.h"
#include "Player.h"
#include "Square.h"
#include "Display.h"

// used for moving a Link
enum Direction {UP, DOWN, LEFT, RIGHT};

// Idea: Board is the only Subject, and its only Observer will be the TextDisplay,
//       hence, I am not writing an abstract Subject/Observer class
class Board {
    std::vector<std::vector<std::unique_ptr<Square>>> theBoard;
    int boardSize = 8;
    int currentPlayer = 0;  // Track the current player (p0 or p1), start with player 0
    bool gameOver = false;  // Flag to indicate if the game is over
    int winner;
    Player players[2];  // since RAIInet is a 2-player game

  public:
    // Constructor:
    Board();
    // Accessor:
    int getCurrentPlayer();
    int getBoardSize();
    int getWinner();

    // Mutator:
    void setWinner(int playerNumber);

    // Other methods:
    void moveLink(Link& link, Direction dir);  // Move a link on the board
    bool isWon();  // Check if a win/lose condition is met
    void Battle(Link& attakcer, Link& defender);
    friend std::ostream &operator<<(std::ostream &out, const Board &board);
};

#endif

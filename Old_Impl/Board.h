#ifndef __BOARD__H__
#define __BOARD__H__

#include <vector>
#include "Link.h"
#include "Player.h"

class Board {
    std::vector<std::vector<std::unique_ptr<Link>>> theBoard;
    int boardSize = 8;  // DOUBLE CHECK THIS
    bool gameOver = false;  // Flag to indicate if the game is over
    Player players[2];  // since RAIInet is a 2-player game
    int currentPlayer = 0;  // Track the current player (p0 or p1)

  public:
    Board();
    
    // ______DOUBLE CHECK: what does init(int n) do? its return type? why a ctor?_________
    // init(int n);  // ctor
    // *--------------------*--------------------*

    void updateBoard();
    void placeLink(const Link& link);  // Place a link on the board
    enum Direction {UP, DOWN, LEFT, RIGHT};
    void moveLink(Link& link, Direction dir);  // Move a link on the board
    bool checkGameState();  // Check if a win/lose condition is met
    void handleLinkBattle(Link& attakcer, Link& defender);

    Player getPlayer();
    Player getOpponent();
    friend std::ostream &operator<<(std::ostream &out, const Board &board);
};

#endif

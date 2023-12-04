#ifndef __BOARD__H__
#define __BOARD__H__

#include <vector>
#include <memory>
#include "Player.h"
#include "Square.h"
#include "Display.h"
#include "Subject.h"

// used for moving a Link
enum Direction {UP, DOWN, LEFT, RIGHT};

// Idea: Board is the only Subject, and its only Observer will be the TextDisplay,
//       hence, I am not writing an abstract Subject/Observer class
class Board : public Subject {
    int boardSize = 8;
    int currentPlayer = 1;  // Track the current player (p0 or p1), start with player 0
    bool isOver = false;  // Flag to indicate if the game is over
    int winner = 0;

  public:
    vector<vector<Square>> board;
    vector<shared_ptr<Player>> players;
    // Constructor:
    Board(string links1, string links2);
    // Accessor:
    // int getCurrentPlayer() const;
    // int getWinner() const;
    // bool getIsOver() const;

    // Mutator:
    void setWinner(int playerNumber);
    void toggleIsOver();
    void switchPlayer();
    void updateGameState(int playerNumber);
    // Other methods:
    void moveLink(char letter, Direction dir);  // Move a link on the board
    bool battle(char letter1, char letter2);
    void downloadLink(char letter);

    int getCurrent() const override;
    bool getIsOver() const override;
	  int getWinner() const override;
	  vector<shared_ptr<Player>> getPlayers() const override;
	  vector<vector<Square>> getBoard() const override;
};

#endif

#ifndef __DISPLAY__H__
#define __DISPLAY__H__

#include <vector>
#include "Board.h"

class Board;

class Display {
    std::vector<std::vector<char>> theDisplay;
    int boardSize = 8;
    int playerNumber;
    std::vector<shared_ptr<Player>> players;
  public:
    Display(int playerNumber, std::vector<shared_ptr<Player>> players);
    void updateDisplay(const Board& b);
    string convert(AbilityName name);
    void printAbilities(int playerNumber);
    friend std::ostream &operator<<(std::ostream &out, const Display &display);
};

#endif

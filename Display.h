#ifndef __DISPLAY__H__
#define __DISPLAY__H__

#include <vector>
#include "Board.h"

class Display : public Observer, public Subject {
    std::vector<std::vector<char>> theDisplay;
    int boardSize = 8;
    int playerNumber;
    int winner = 0;
    bool isOver = false;

  public:
    std::vector<shared_ptr<Player>> players;
    Display(int playerNumber; std::vector<shared_ptr<Player>> players);
    void notify(Subject& whichPlayer);
    friend std::ostream &operator<<(std::ostream &out, const Display &display);

    int getCurrent() override;
    bool getIsOver() override;
	  int getWinner() override;
	  std::vector<shared_ptr<Player>> getPlayers() override;
	  std::vector<std::vector<Square>> getBoard() override;
};

#endif

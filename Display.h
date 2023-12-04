#ifndef __DISPLAY__H__
#define __DISPLAY__H__

#include <vector>
#include "Board.h"
#include "Observer.h"
#include "Subject.h"

class Display : public Observer, public Subject {
    std::vector<std::vector<char>> theDisplay;
    int boardSize = 8;
    int playerNumber;
    int winner = 0;
    bool isOver = false;

  public:
    std::vector<shared_ptr<Player>> players;
    Display(int playerNumber, std::vector<shared_ptr<Player>> players);
    int getCurrPlayer() const;
    void notify(const Subject& whichPlayer) override;
    friend std::ostream &operator<<(std::ostream &out, const Display &display);

    int getCurrent() const override;
    bool getIsOver() const override;
	  int getWinner() const override;
	  vector<shared_ptr<Player>> getPlayers() const override;
    vector<vector<Square>> getBoard() const override;
};

#endif

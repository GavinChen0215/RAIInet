#ifndef _SUBJECT_H_
#define _SUBJECT_H_
#include <memory>
#include <vector>
#include "Square.h"
using namespace std;

class Observer;
class Player;

class Subject {
	vector<shared_ptr<Observer>> observers;
  public:
    Subject();
	void attachObserver(std::shared_ptr<Observer> ob);
	void detachObserver(std::shared_ptr<Observer> ob);
	void notifyObservers() const;
    virtual ~Subject() = 0;
	virtual int getCurrent() = 0;
    virtual bool isWon() = 0;
	virtual int whoWon() = 0;
	virtual std::vector<shared_ptr<Player>> getPlayers() = 0;
	virtual std::vector<std::vector<Square>> getBoard() = 0;
};

#endif

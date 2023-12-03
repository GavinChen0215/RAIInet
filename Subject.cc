#include "Observer.h"
#include "Subject.h"

void Subject::attachObserver(std::shared_ptr<Observer> ob) { observers.emplace_back(ob); }

void Subject::detachObserver(std::shared_ptr<Observer> ob) {
    for (auto it = observers.begin(); it != observers.end();) {
      if (*it == ob) {
        it = observers.erase(it);
      }
    }
}

void Subject::notifyObservers() {
	for (auto it : observers) {
		notify(*it);
	}
}

Subject::~Subject() {}

#include "Observer.h"
#include "Subject.h"

Subject::Subject() {}

void Subject::attachObserver(std::shared_ptr<Observer> ob) { observers.emplace_back(ob); }

void Subject::detachObserver(std::shared_ptr<Observer> ob) {
    for (auto it = observers.begin(); it != observers.end();) {
        if (*it == ob) {
            it = observers.erase(it);
        } else {
            ++it;
        }
    }
}

void Subject::notifyObservers() const {
	for (auto it : observers) {
		it->notify(*this);
	}
}

Subject::~Subject() {}

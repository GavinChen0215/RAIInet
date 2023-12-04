#ifndef __OBSERVER__H__
#define __OBSERVER__H__

class Subject;

class Observer {
  public:
    virtual void notify(const Subject& whichPlayer) = 0;
	  virtual ~Observer() = default;
};

#endif

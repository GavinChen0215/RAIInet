#ifndef _DECORATOR_H_
#define _DECORATOR_H_
#include "Ability.h"

class Decorator: public Ability {
 protected:
  Ability *component;
 public:
  Decorator(Ability *component);
  virtual ~Decorator();
};

#endif
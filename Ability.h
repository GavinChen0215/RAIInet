#ifndef __ABILITY__H__
#define __ABILITY__H__

#include "Link.h"
#include "Board.h"

// Decorator Pattern will be used here
class Ability {  // Abstract superclass
    bool state;

  public:
    virtual void use() = 0;  // pure virtual method
    bool getState() const;
    void toggleState() const;
    virtual ~Ability() {}
};

class LinkBoost : public Ability {
    Link link;
public:
    void use() override;
};

class Firewall : public Ability {
    Board *board;
    int r, c;
public:
    void use() override;
};

class Download : public Ability {
    Link link;
    Board *board;
public:
    void use() override;
};

class Polarize : public Ability {
    Link link;
public:
    void use() override;
};

class Scan : public Ability {
    Link link;
    Board *board;
public:
    void use() override;
};

#endif

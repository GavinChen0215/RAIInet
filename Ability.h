#ifndef __ABILITY__H__
#define __ABILITY__H__

#include <string>

enum AbilityName {LinkBoost, Firewall, Download, Polarize, Scan};

class Ability {
    AbilityName name;
    int id;
    bool isUsed = false;  // default
  public:
    // Constructor
    Ability(int id, char initial);

    // Accessor
    AbilityName getName() const;
    int getID() const;
    bool getIsUsed() const;

    // Mutator
    void toggleIsUsed();

    // Other
    std::string convert(AbilityName name);
};

#endif


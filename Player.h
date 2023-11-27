#ifndef __PLAYER__H__
#define __PLAYER__H__

#include <vector>
#include "Link.h"
#include "Ability.h"

class Player {
    int playerNumber;
    std::vector<std::unique_ptr<Link>> links;  // The Links that a Player owns
    std::vector<std::unique_ptr<Ability>> abilities;  // The Abilities that a Player owns
    int downloadedData;  // the number of data that has been downloaded
    int donwloadedVirus;  // the number of virus that has been downloded
  public:
    Player(int playerNumber);
    int getData() const;
    int getViruses() const;
    void downloadLink(Link &link); // Handle downloading a link
    void useAbility(int ID); // Use an ability
    void addLink(const Link& link);
    void addAbility(const Ability& ability);
};

#endif

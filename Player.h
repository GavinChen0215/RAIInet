#ifndef __PLAYER__H__
#define __PLAYER__H__

#include <vector>
#include "Link.h"
// #include "Ability.h"

class Player {
    int playerNumber;  // either 0 or 1, indicating the two players in the game
    std::vector<std::unique_ptr<Link>> links;  // array of unique_ptr to Links that a Player owns
    int downloadedData;  // the number of data that has been downloaded
    int downloadedVirus;  // the number of virus that has been downloded

  public:
    Player(int playerNumber); // ctor
    int getData() const;  // # of data downloaded
    int getVirus() const; // # of virus donwloaded

    void downloadLink(Link &link); // Handle downloading a link
    
    // --- DOUBLE-CHECK addLink ---
    void addLink(const Link& link);
};

#endif

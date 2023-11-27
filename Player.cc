#include "Player.h"

// constructor
Player::Player(int playerNumber): 
    playerNumber{playerNumber},
    downloadedData{0},
    downloadedVirus{0} {}

int Player::getData() const { return downloadedData; }
int Player::getViruses() const { return downloadedVirus; }

void downloadLink(Link &link) {
    // FILL IN
}
void useAbility(int ID) {
    // FILL IN
}
void addLink(const Link& link) {
    // FILL IN
}
void addAbility(const Ability& ability) {
    // FILL IN
}


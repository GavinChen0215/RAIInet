#include "Player.h"

// constructor
Player::Player(int playerNumber): 
    playerNumber{playerNumber},
    downloadedData{0},
    downloadedVirus{0} {}

int Player::getData() const { return downloadedData; }
int Player::getViruses() const { return downloadedVirus; }

void downloadLink(Link &link) {
    link.toggleState();
    link.toggleVisbility();
}
void useAbility(int ID) {
    // FILL IN
}
void addLink(Link& link) {
    links.emplace_back(std::make_unique<Link>(&link));
}
void addAbility(Ability& ability) {
    abilities.emplace_back(std::make_unique<Ability>(&abilities));
}


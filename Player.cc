#include "Player.h"

// constructor
Player::Player(int playerNumber): 
    playerNumber{playerNumber},
    downloadedData{0},
    downloadedVirus{0} {}

int Player::getData() const { return downloadedData; }
int Player::getVirus() const { return downloadedVirus; }

void Player::downloadLink(Link &link) {
    link.toggleState();
    link.toggleVisbility();
    link.getType() == 'D' ? ++downloadedData : ++downloadedVirus;
}

void Player::addLink(const Link& link) {
    links.emplace_back(std::make_unique<Link>(&link));
}

// void Player::useAbility(int ID) {
//     // FILL IN
// }

// void Player::addAbility(const Ability& ability) {
//     abilities.emplace_back(std::make_unique<Ability>(&ability));
// }


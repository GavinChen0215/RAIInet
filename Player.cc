#include "Player.h"

Player::Player(int playerNumber, string AllLinks): playerNumber{playerNumber} {
    char letter = (playerNumber == 1) ? 'a' : 'b';
    int n = linkLetters.length();
    int i = 0;
    for (int j = 0; j < n; j+=2) {
        char type = AllLinks[j];
        int strength = AllLinks[j+1] - '0';
        links.emplace_back(make_shared<Link>(type, letter, strength, playerNumber));
        links[i]->setCol(i);
        if (playerNumber == 1) {
            if (i == 3 || i == 4) {
                links[i]->setRow(1);
            } else {
                links[i]->setRow(0);
            }
        } else {
            if (i == 3 || i == 4) {
                links[i]->setRow(6);
            } else {
                links[i]->setRow(7);
            }
        }
        ++i;
    }
}

int Player::getPlayerNumber() const { return playerNumber; }
int Player::getData() const { return downloadedData; }
int Player::getViruses() const { return downloadedVirus; }
int Player::getNumAbility() const { return numAbility; }
void Player::IncreData() { ++downloadedData; }
void Player::IncreViruses() { ++downloadedVirus; }
void Player::DecreAbility() { --numAbility; }


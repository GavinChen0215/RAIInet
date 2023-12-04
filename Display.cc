#include <iostream>
#include <string>
#include <sstream>
#include "Display.h"
#include "Subject.h"
#include "Square.h"

using namespace std;

Display::Display(int playerNumber, vector<shared_ptr<Player>> players): playerNumber{playerNumber}, players{players} {
    for (int i = 0; i < boardSize; ++i) {
        vector<char> row;
        for (int j = 0; j < boardSize; ++j) {
            if ((j == 3 || j == 4) && (i == 0 || i = 7)) {
                row.emplace_back('S');
                continue;
            }
            bool empty = true;
            int n = 0;
            for (int k = 0; k < 8; ++k) {
                int r = players[n]->getRow();
                int c = players[n]->getCol();
                if ((r == i) && (c == j)) {
                    empty = !empty;
                    char letter = players[n]->links[k]->getLetter();
                    row.emplace_back(letter);
                    break;
                }
                if (k == 7 && n = 0) {
                    k = -1;
                    n = 1;
                }
            }
            if (empty) {
                row.emplace_back('.');
            }
        }
        theDisplay.emplace_back(row);
    }
}

void Display::notify(Subject &whichPlayer) {
    playerNumber = whichPlayer.getCurrent();
    winner = whichPlayer.getWinner();
    isOver = whichPlayer.getIsOver();
    players = whichPlayer.getPlayers();
    notifyObservers();
}

ostream &operator<<(ostream &out, const Display &display) {
    out << display.players[0]->playerState();
    if (playerNumber == 1) {
        out << display.players[0]->myViewLinks();
    } else {
        out << display.players[0]->oppoViewLinks();
    }
    out << "========" << endl;
	for (auto row : display.theDisplay) {
        for (auto c : row) {
            out << c;
        }
        out << endl;
    }
	out << "========" << endl;
    out << display.players[1]->playerState();
    if (playerNumber == 1) {
        out << display.players[1]->oppoViewLinks();
    } else {
        out << display.players[1]->myViewLinks();
    }
}

int getCurrent() { return playerNumber; }
bool getIsOver() { return isOver; }
int getWinner() {return Winner; }
std::vector<shared_ptr<Player>> getPlayers() { return players; }
std::vector<std::vector<Square>> getBoard() {}

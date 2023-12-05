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
            if ((j == 3 || j == 4) && (i == 0 || i == 7)) {
                row.emplace_back('S');
                continue;
            }
            bool empty = true;
            int n = 0;
            for (int k = 0; k < 8; ++k) {
                int r = players[n]->links[k]->getRow();
                int c = players[n]->links[k]->getCol();
                if ((r == i) && (c == j)) {
                    empty = !empty;
                    char letter = players[n]->links[k]->getLetter();
                    row.emplace_back(letter);
                    break;
                }
                if (k == 7 && n == 0) {
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

void Display::updateDisplay(const Board& b) {
    // Assuming Board provides a method to get its current state as a 2D vector
    theDisplay.clear();
    for (const auto& row : b.board) {
        vector<char> displayRow;
        for (const auto& square : row) {
            displayRow.emplace_back(square.getContent());
        }
        theDisplay.emplace_back(displayRow);
    }
    playerNumber = b.getCurrent();
}

string Display::convert(AbilityName name) {
    if (name == LinkBoost) return "LinkBoost";
    else if (name == Firewall) return "Firewall";
    else if (name == Download) return "Download";
    else if (name == Polarize) return "Polarize";
    else return "Scan";
}

void Display::printAbilities(int playerNumber) {
    cout << "              Player " << playerNumber << endl;
    cout << "----------- ABILITY CARD ------------" << endl;
    for (int i = 0; i < 5; ++i) {
        bool isUsed = players[playerNumber - 1]->abilities[i]->getIsUsed();
        string use;
        AbilityName abName = players[playerNumber - 1]->abilities[i]->getName();
        (isUsed)? use = "Yes" : use = "No";
        if (abName == Scan) {
            cout<<"ID: "<<i+1<<"   Name: "<<convert(abName)<<"       Used: "<<use<<endl; // just to make it align => looks better
        } else if (abName == LinkBoost) {
            cout<<"ID: "<<i+1<<"   Name: "<<convert(abName)<<"  Used: "<<use<<endl; // just to make it align => looks better
        } else {
            cout << "ID: " << i + 1 <<
            "   Name: " << convert(abName) <<
            "   Used: " << use << endl;
        }
    }
    cout << "-------------------------------------" << endl;
}

ostream &operator<<(ostream &out, const Display &display) {
    out << display.players[0]->playerState();
    int number = display.playerNumber;
    if (number == 1) {
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
    if (number == 1) {
        out << display.players[1]->oppoViewLinks();
    } else {
        out << display.players[1]->myViewLinks();
    }
    return out;
}

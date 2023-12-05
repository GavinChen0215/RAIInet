#include <iostream>
#include <string>
#include <memory>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <random>
#include <chrono>
#include "Board.h"
#include "DisplayBoard.h"


using namespace std;

int main(int argc, const char* argv[]) {
    string p1Link, p2Link;
    string p1Ab = "LFDSP", p2Ab = "LFDSP";
    bool graphics = false;

    // ===== Randomize the Link that each Player gets =====
    std::vector<std::string> randomLink = {"V1", "V2", "V3", "V4", "D1", "D2", "D3", "D4"};
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count(); // from: CS246 shuffle.cc
	std::default_random_engine rng{seed};

    std::shuffle( randomLink.begin(), randomLink.end(), rng );
    for (std::string const &i : randomLink) {
        p1Link += i;
    }
    
    std::shuffle( randomLink.begin(), randomLink.end(), rng );
    for (std::string const &i : randomLink) {
        p2Link += i;
    }
    // ===== Randomization Ends =====


    // ===== Set Up Link/Ability =====
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        // format:  -link1 <placement-file>
        if (arg == "-link1") {  
            p1Link = ""; // reset since user is going to customize the Link
            ifstream link1Content {argv[i + 1]};  // this is the <placement-file>
            string s;
            while(link1Content >> s) {
                p1Link += s;
            }
            ++i;  // to skip over the argument for <placement-file>
        } else if (arg == "-link2") { 
            p2Link = "";
            ifstream link2Content {argv[i + 1]};
            string s;
            while(link2Content >> s) {
                p2Link += s;
            }
            ++i;
        } else if (arg == "-ability1") {
            string temp = argv[i + 1];
            if (temp.length() != 5) {
                cout << "Incorrect set up of ability1" << endl;
                return 1;
            }
            p1Ab = temp;
            ++i;
        } else if (arg == "-ability2") {
            string temp = argv[i + 1];
            if (temp.length() != 5) {
                cout << "Incorrect set up of ability2" << endl;
                return 1;
            }
            p2Ab = temp;
            ++i;
        } else if (arg == "-graphics") {
            graphics = true;
        }
    }
    // ===== Set Up Ends =====


    // Initialization
    Board board{p1Link, p2Link, p1Ab, p2Ab};
    shared_ptr<DisplayBoard> display = make_shared<DisplayBoard>(1, graphics, board.players);  // start with player 1
    
    // ====== START GAME ======
    istream *input = &cin;  // the input source in default to be cin
    int abilityLeft = 1; // # of ability that a Player can use in each turn
    string cmd;
    cout << "___________________________" << endl;
    cout << *display;
    cout << "___________________________" << endl;
    
    while (*input >> cmd) {
        if (cmd == "quit") break;  // exit the game
        else if (cmd == "sequence") {  // change the input source to <filename>
            string filename;
            cin >> filename;
            ifstream cmdFile {filename};
            input = &cmdFile;
            continue;  // now, the commands used in the game loop are from <filename>, not cin
        } else if (cmd == "board") {  // display the Board
            cout << "___________________________" << endl;
            cout << *display;
            cout << "___________________________" << endl;
            cout << endl;
        } else if (cmd == "move") {
            char letter;
            *input >> letter; // which Link to move
            string direction;
            *input >> direction;
            Direction dir;
            if ((board.getCurrent() == 1 && (!(letter >= 'a' && letter <= 'h'))) || 
            (board.getCurrent() == 2 && (!(letter >= 'A' && letter <= 'H')))) {
                cout << "Invalid move! You don't own: " << letter << endl;
                cout << endl;
                continue;
            }
            if (direction == "up") {
                dir = Direction::UP;
            } else if (direction == "down") {
                dir = Direction::DOWN;
            } else if (direction == "left") {
                dir = Direction::LEFT;
            } else if (direction == "right") {
                dir = Direction::RIGHT;
            } else {
                cout << "Invalid Direction" << endl;
                cout << endl;
                continue;
            }
            board.moveLink(letter, dir);
            display->updateDisplay(board);
            cout << "___________________________" << endl;
            cout << *display;
            cout << "___________________________" << endl;
            cout << endl;
            abilityLeft = 1;  // switch turn, reset
            if (board.getIsOver()) {
                cout << "Player " << board.getWinner() << "Won!" << endl;
                break;
            }
        } else if (cmd == "ability") {
            if (abilityLeft == 0) {  // already used an ability in the current turn
                cout << "You have already used an ability in this turn" << endl;
                continue;
            }
            int abID;
            *input >> abID;
            int successAb = board.useAbility(abID, *input);
            display->updateDisplay(board);
            cout << *display;
            if (successAb) abilityLeft = 0;
        } else if (cmd == "abilities") {  // display the list of abilities that a Player has
            display->printAbilities(board.getCurrent());
        } else {
            cout << "Invalid Command. Please Try Again: ";
        }
    } // while
    return 0;
} // main


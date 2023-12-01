class Link;

/*
#include "Link.h"
*/

class Observer {
 public:
  virtual void notify(Link *lp) = 0;  // c is the Cell that called the notify method
  virtual ~Observer() = default;
};

/*
#include "Observer.h"
*/

class Link: public Observer {
    char type;  // 'D' for data, 'V' for virus
    char letter; // a-h / A-H
    int strength;  // strength of the Link, [1, 2, 3, 4]
    int row, col;  // position of the Link
    // the following fields have default values
    int range = 1;  // how many grid can a Link move at a time
    bool visibility = false;  // whether the Link is visible to the opponent
    bool isDownload = false;  // whether the Link has been downloaded

    std::vector<Observer*> observers;  // Observers to be notified
  public:
    // constructor, default: range == 1, visibility == isDownload == false
    Link(char type, char letter; int strength, int r, int c):
        type{type}, letter{letter}, strength{strength}, row{r}, col{c} {}
    // accessor
    char getType() const { return type; } 
    char getLetter() const { return letter; }
    int getStrength() const { return strength; }
    int getRow() const { return row; }
    int getCol() const { return col; }
    int getRange() const { return range; }
    bool getState() const { return isDownload; }
    bool getVisibility() const { return visibility; }
    // mutator
    void toggleType() {
        (type == 'D') ? type = 'V' : type = 'D';  // change 'D' to 'V', 'V' to 'D'
        notifyObservers();
    } 
    void setLetter(char c) {
        letter = c;
    }
    void boostRange() {
        range = 2;
        notifyObservers();
    }  // change the range to 2, after using "LinkBoost"
    void setRow(int r) {
        row = r;
        notifyObservers();
    }
    void setCol(int c) {
        col = c;
        notifyObservers();
    }
    void toggleState() {
        isDownload = true; // negate the isDownload field
        notifyObservers();
    }
    void toggleVisbility() {
        visibility = true;  // negate the visibility field
        notifyObservers();
    }
    void attachObserver(Observer* obs) {
        observers.emplace_back(obs);
    }

    void detachObserver(Observer* obs) {
        observers.erase(std::remove(observers.begin(), observers.end(), obs), observers.end());
    }

    void notifyObservers() {
        for (Observer* obs : observers) {
            obs->notify(this);
        }
    }
};

/*
#include <vector>
#include <memory>
#include "Link.h" 
#include "Ability.h"
*/

class Player {
    int playerNumber;
    std::vector<std::unique_ptr<Link>> links;  // The Links that a Player owns
    std::vector<std::unique_ptr<Ability>> abilities;  // The Abilities that a Player owns
    int downloadedData = 0;  // the number of data that has been downloaded
    int downloadedVirus = 0;  // the number of virus that has been downloded
  public:
    Player(int playerNumber): playerNumber{playerNumber} {}

    int getData() const { return downloadedData; }
    int getViruses() const { return downloadedVirus; }
    Link* getLink(char letter) {
        for (auto& link : links) {
            if (link->getLetter() == letter) {
                return link.get();
            }
        }
        return nullptr;
    }
   void downloadLink(Link* link) {
        if (link) {
            link->toggleState();
            link->toggleVisbility();
            if (link->getType() == 'D') {
                ++downloadedData;
            } else {
                ++downloadedVirus;
            }
        }
    }
    void useAbility(int ID, Board* b) {
        abilities[ID - 1]->use(b);
    }
    void addLink(const Link link) {
        links.emplace_back(std::make_unique<Link>(link));
    }
    void addAbility(const Ability ability) {
        abilities.emplace_back(std::make_unique<Ability>(ability));
    } 
};

class Board;
class Ability {  // Abstract superclass
    bool state = true;
  public:
    virtual void use(Board* board) = 0;  // pure virtual method
    bool getState() const { return state; }
    void toggleState() { state = false; }
    virtual ~Ability() = default;
};

class LinkBoost : public Ability {
public:
    void use(Board* board) override {
        char l;
        cin >> l;
        int currPlayer = board->getCurrent();
        if ('a' <= l && l <= 'h' && currPlayer == 0) {
            board->getPlayer(0).getLink(l).boostRange();
        } else if ('A' <= l && 'H' <= l && currPlayer == 1) {
            board->getPlayer(1).getLink(l).boostRange();
        } else {
            std::cerr << "Please provide a correct link value" << std::endl;
        }
    }
};

class Firewall : public Ability {
public:
    Firewall(Board* b): board(b) {}

    void use() override {
        int r, c;
        cin >> r >> c;

    }
};

class Download : public Ability {

public:
    void use(Board* board) override {
        char l;
        cin >> l;
        int currPlayer = board->getCurrent();
        if ('a' <= l && l <= 'h' && currPlayer == 1) {
            Link link = board->getPlayer(0).getLink(l);
            board->getPlayer(1).downloadLink(link);
        } else if ('A' <= l && 'H' <= l && currPlayer == 0) {
            Link link = board->getPlayer(1).getLink(l);
            board->getPlayer(0).downloadLink(link);
        } else {
            std::cerr << "Please provide a correct link value" << std::endl;
        }
    }
};

// Polarize Decorator
class Polarize : public Ability {
public:

    void use(Board* board) override {
        char l;
        cin >> l;
        if ('a' <= l && l <= 'h') {
            board->getPlayer(0).getLink(l).toggleType();
        } else if ('A' <= l && 'H' <= l) {
            board->getPlayer(1).getLink(l).toggleType();
        } else {
            std::cerr << "Please provide a correct link value" << std::endl;
        }
    }
};

// Scan Decorator
class Scan : public Ability {

public:

    void use(Board* board) override {
        char l;
        cin >> l;
        if ('a' <= l && l <= 'h') {
            board->getPlayer(0).getLink(l).toggleVisbility();
        } else if ('A' <= l && 'H' <= l) {
            board->getPlayer(1).getLink(l).toggleVisbility();
        } else {
            std::cerr << "Please provide a correct link value" << std::endl;
        }
    }
};

// 3 more new abilities

/*
#include <vector>
#include <iostream>
#include <memory>
#include "Link.h"   
#include "Player.h" 
#include "Observer.h"
*/

class Board : public Observer{
    std::vector<std::vector<std::unique_ptr<Link>>> theBoard;
    int boardSize = 8;  // Board size for RAIInet
    bool isWon = false; 
    Player players[2];
    int currentPlayer = 0;
    std::unique_ptr<Display> display = nullpter;

public:
    // Constructor
    Board() {
        theBoard.resize(boardSize, std::vector<std::unique_ptr<Link>>(boardSize, nullptr));
    }
    void setCurrent() {currentPlayer = 1 - currentPlayer;}
    int getCurrent() const { return currentPlayer;}
    // Initialize the board with players and links
    void init() {
        // init links
        for (int i = 0; i < 2; ++i) {
            players[i] = Player(i);
            std::vector<int> str = {1, 2, 3, 4, 1, 2, 3, 4};
            std::vector<char> types = {'D', 'D', 'D', 'D', 'V', 'V', 'V', 'V'};
            std::vector<char> letter1 = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
            std::vector<char> letter2 = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
            
            // Use a random device to generate a seed
            std::random_device rd;  
            std::mt19937 rng(rd()); // Standard mersenne_twister_engine seeded with rd()
            // Shuffle the vector
            std::shuffle(str.begin(), str.end(), rng);

            std::mt19937 rng(rd()); // Standard mersenne_twister_engine seeded with rd()
            // Shuffle the vector
            std::shuffle(types.begin(), types.end(), rng);

            std::mt19937 rng(rd()); // Standard mersenne_twister_engine seeded with rd()
            // Shuffle the vector
            std::shuffle(letter1.begin(), letter1.end(), rng);

            std::mt19937 rng(rd()); // Standard mersenne_twister_engine seeded with rd()
            // Shuffle the vector
            std::shuffle(letter2.begin(), letter2.end(), rng);

            for (int j = 0; j < 8; ++j) {
                if (i = 0) {
                    if (j = 3 || j = 4) {
                        Link l{types[j], letter1[j], str[j], 1, j};
                    } else {
                        Link l{types[j], letter1[j], str[j], 0, j};
                    }
                } else {
                    if (j = 3 || j = 4) {
                        Link l{types[j], letter2[j], str[j], 7, j};
                    } else {
                        Link l{types[j], letter2[j], str[j], 8, j};
                    }
                }
                l.attachObserver(this);
                players[i].addLink(l);
                placeLink(*link, link->getRow(), link->getCol());
            }
        }
    }

    // Update the board state
    void updateBoard() {
        // Implement the logic to update the board state
        // Checking for battles, updating link positions, etc.
    }

    // Place a link on the board
    void placeLink(const Link& link, int row, int col) {
        if (row >= 0 && row < boardSize && col >= 0 && col < boardSize) {
            if (!theBoard[row][col]) {
                // Place a new link only if the cell is empty
                theBoard[row][col] = std::make_unique<Link>(link);
            } else {
                std::cerr << "Cell is already occupied" << std::endl;
            }
        } else {
            std::cerr << "Invalid position" << std::endl;
        }
    }

    // Enum for direction of movement
    enum Direction { UP, DOWN, LEFT, RIGHT };

    // Move a link on the board
    void moveLink(Link& link, Direction dir) {
        int row = link.getRow();
        int col = link.getCol();
        int newRow = row;
        int newCol = col;

        switch (dir) {
            case UP:    newRow -= link.getRange(); break;
            case DOWN:  newRow += link.getRange(); break;
            case LEFT:  newCol -= link.getRange(); break;
            case RIGHT: newCol += link.getRange(); break;
        }

        if (newRow >= 0 && newRow < boardSize && newCol >= 0 && newCol < boardSize) {
            if (!theBoard[newRow][newCol]) {
                theBoard[newRow][newCol] = std::move(theBoard[row][col]);
                link.setRow(newRow);
                link.setCol(newCol);
            } else {
                // Handle the situation when the new cell is occupied (possible battle)
                handleLinkBattle(link, *(theBoard[newRow][newCol]));
            }
        } else {
            std::cerr << "Move out of bounds" << std::endl;
        }
    }

    // Check if a win/lose condition is met
    bool checkGameState() {
        return isWon;
    }

    // Handle a battle between two links
    void handleLinkBattle(Link& attacker, Link& defender) {
        int a = attacker.getStrength();
        int d = defender.getStrength();
        if (a >= d) {
            players[currentPlayer].downloadLink(defender);
        } else {
            players[1 - currentPlayer].downloadLink(defender);
        }
    }

    // Getters for players
    Player& getPlayer(int playerNum) {
        return players[playerNum];
    }

    friend std::ostream &operator<<(ostream &out, const Board *b) {
        if (b.td) out << *(b.td);
        return out;
    }
};

/*
#include <vector>
#include <iostream>
#include "Observer.h"
#include "Link.h"
*/

class Display : public Observer {
    std::vector<std::vector<char>> theDisplay;
    const int boardSize = 8;  // Assuming board size is 8x8

public:
    // Constructor
    Display() : theDisplay(boardSize, std::vector<char>(boardSize, '.')) {}

    // Observer's notify method implementation
    void notify(Link* lp) override {
        if (lp) {
            int row = lp->getRow();
            int col = lp->getCol();
            char displayChar = '.';

            if (!lp->getState()) {
                // Display the link's letter if it is visible and not downloaded
                displayChar = lp->getLetter();
            }

            if (row >= 0 && row < boardSize && col >= 0 && col < boardSize) {
                theDisplay[row][col] = displayChar;
            }
        }
    }

    void resetDisplay() {
        for (auto& row : theDisplay) {
            std::fill(row.begin(), row.end(), '.');
        }
    }

    friend ostream &operator<<(ostream &out, const Display &d) {
    // Output the display grid to the stream based on the state of every cell
    out << "========" << endl;
    for (auto row : d.theDisplay) {
        for (char link : row) {
            out << link;
        }
        out << endl;
    }
    out << "========" << endl;
    return out;
};

// Overload the output operator to display the board
std::ostream &operator<<(std::ostream &out, const Display &display) {
    display.render();
    return out;
}

/*
#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include "Board.h" 
#include "Display.h"
#include "Link.h"
*/
int main(int argc, char* argv[]) {
    std::unique_ptr<Board> board = std::make_unique<Board>();
    Display display;  // The display for the board

    // Parse command-line arguments for setup
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-ability1") {
            // Example: Process abilities for player 1
        } else if (arg == "-ability2") {
            // Example: Process abilities for player 2
        } else if (arg == "-link1") {
            // Example: Process link placement for player 1
        } else if (arg == "-link2") {
            // Example: Process link placement for player 2
        }
        // -graphics later
    }

    board->init();  // Initialize the board with players and links
    display.resetDisplay();  // Reset the display for the new game

    std::string cmd;
    int currentPlayer = 0;  // Start with player 0

    // Game loop
    while (true) {
        std::cout << "Player " << currentPlayer + 1 << ", enter command: ";
        std::cin >> cmd;

        if (cmd == "move") {
            char value;
            std::cin >> value;

            string direction;
            std::cin >> direction;
            Board::Direction dir = Board::UP; // Default direction
            // Convert char to Direction enum
            switch (direction) {
                case 'up': dir = Board::UP; break;
                case 'down': dir = Board::DOWN; break;
                case 'left': dir = Board::LEFT; break;
                case 'right': dir = Board::RIGHT; break;
            }
            Link& movingLink = board->getPlayer(currentPlayer).getLink(value); // Get link to move
            board->moveLink(movingLink, dir); // Move the link
            display.updateDisplay();
            currentPlayer = 1 - currentPlayer; // Switch players
        } else if (cmd == "ability") {
            int ID;
            std::cin >> ID;
            board->getPlayer(currentPlayer).useAbility(ID); // Use an ability
            display.updateDisplay();
        } else if (cmd == "abilities") {
            // Display available abilities
        } else if (cmd == "board") {
            // Display the current state of the board
            std::cout << *board;
        } else if (cmd == "sequence") {
            std::string filename;
            std::cin >> filename;
            // Execute commands from a file
        } else if (cmd == "quit") {
            std::cout << "Quitting the game." << std::endl;
            break;
        } else {
            std::cerr << "Invalid command." << std::endl;
        }

        if (board->checkGameState()) {
            std::cout << "Player " << currentPlayer + 1 << " wins!" << std::endl;
            break;
        }
        cout << display;
    }
    return 0;
}
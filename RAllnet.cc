class Link {
    char type;  // 'D' for data, 'V' for virus
    int strength;  // strength of the Link, [1, 2, 3, 4]
    int row, col;  // position of the Link
    // the following fields have default values
    int range = 1;  // how many grid can a Link move at a time
    bool visibility = false;  // whether the Link is visible to the opponent
    bool isDownload = false;  // whether the Link has been downloaded

  public:
    // constructor, default: range == 1, visibility == isDownload == false
    Link(char type, int strength, int r, int c):
        type{type}, strength{strength}, row{r}, col{c} {}
    // accessor
    char getType() const { return type; } 
    char getStrength() const { return strength; }
    int getRow() const { return row; }
    int getCol() const { return col; }
    int getRange() const { return range; }
    bool getState() const { return isDownload; }
    bool getVisibility() const { return visibility; }
    // mutator
    void toggleType() {
        (type == 'D') ? type = 'V' : type = 'D';  // change 'D' to 'V', 'V' to 'D'
    } 

    void boostRange() { range = 2; }  // change the range to 2, after using "LinkBoost"
    void setRow(int r) { row = r; }
    void setCol(int c) { col = c; }
    void toggleState() {
        isDownload = true;  // negate the isDownload field
    }
    void toggleVisbility() {
        visibility = true;  // negate the visibility field
    }
};

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
    Link getLink(int n) {
        return *(links[n]);
    }
    void downloadLink(Link &link) {
        link.toggleState();
        link.toggleVisbility();
        if (link.getType() == 'D') {
            ++downloadedData;
        } else {
            ++downloadedVirus;
        }
    }
    void useAbility(int ID) {
        abilities[ID - 1]->use();
    }
    void addLink(const Link link) {
        links.emplace_back(std::make_unique<Link>(link));
    }
    void addAbility(const Ability ability) {
        abilities.emplace_back(std::make_unique<Ability>(ability));
    }
};

class Ability {  // Abstract superclass
    bool state = true;

  public:
    virtual void use() = 0;  // pure virtual method
    bool getState() const { return state; }
    void toggleState() { state = !state; }
    virtual ~Ability() {}
};

class Decorator: public Ability {
 protected:
    std::unique_ptr<Ability> component;
 public:
    Decorator(std::unique_ptr<Ability> comp) : component(std::move(comp)) {}
    virtual ~Decorator() {}

    void use() override {
        if (component) {
            component->use();  // Delegate to the wrapped component
        }
    }
};

class LinkBoost : public Decorator {
    Board* board;
public:
    LinkBoost(std::unique_ptr<Ability> comp, Board* b):
        Decorator(std::move(comp)), board(b) {}
    void use() override {
        char l;
        cin >> l;
        int n = -1;
        int currPlayer = board->getCurrent();
        if ('a' <= l && l <= 'h' && currPlayer == 0) {
            n = l - 'a';
        } else if ('A' <= l && 'H' <= l && currPlayer == 1) {
            n = l - 'A';
        } else {
            std::cerr << "Please provide a correct link value" << std::endl;
        }
        if (n != -1) {
            board->getPlayer(currPlayer).getLink(n).boostRange();
        }
    }
};

class Firewall : public Decorator {
    Board* board;

public:
    Firewall(std::unique_ptr<Ability> comp, Board* b):
        Decorator(std::move(comp)), board(b) {}

    void use() override {
        int r, c;
        cin >> r >> c;
        if (board) {
            // Example: Implement Firewall logic here
            std::cout << "Activating Firewall at (" << r << ", " << c << ")" << std::endl;
        }
    }
};

class Download : public Decorator {
    Board* board;

public:
    Download(std::unique_ptr<Ability> comp, Board* b): 
        Decorator(std::move(comp)), board(b) {}

    void use() override {
        char l;
        cin >> l;
        int n = -1;
        int currPlayer = board->getCurrent();
        if ('a' <= l && l <= 'h' && currPlayer == 1) {
            n = l - 'a';
        } else if ('A' <= l && 'H' <= l && currPlayer == 0) {
            n = l - 'A';
        } else {
            std::cerr << "Please provide a correct link value" << std::endl;
        }
        if (n != -1) {
            Link link = board->getPlayer(1 - currPlayer).getLink(n);
            board->getPlayer(currPlayer).downloadLink(link);
        }
    }
};

// Polarize Decorator
class Polarize : public Decorator {
    Board* board;
public:
    Polarize(std::unique_ptr<Ability> comp, Board* b):
        Decorator(std::move(comp)), board(b) {}

    void use() override {
        char l;
        cin >> l;
        int n = -1;
        int player;
        if ('a' <= l && l <= 'h') {
            n = l - 'a';
            player = 0;
        } else if ('A' <= l && 'H' <= l) {
            n = l - 'A';
            player = 1;
        } else {
            std::cerr << "Please provide a correct link value" << std::endl;
        }
        if (n != -1) {
            board->getPlayer(player).getLink(n).toggleType();
        }
    }
};

// Scan Decorator
class Scan : public Decorator {
    Board* board;

public:
    Scan(std::unique_ptr<Ability> comp, Board* b):
        Decorator(std::move(comp)), board(b) {}

    void use() override {
        char l;
        cin >> l;
        int n = -1;
        int player;
        if ('a' <= l && l <= 'h') {
            n = l - 'a';
            player = 0;
        } else if ('A' <= l && 'H' <= l) {
            n = l - 'A';
            player = 1;
        } else {
            std::cerr << "Please provide a correct link value" << std::endl;
        }
        if (n != -1) {
            board->getPlayer(player).getLink(n).toggleVisbility();
        }
    }
};

// 3 more new abilities

class Board {
    std::vector<std::vector<std::unique_ptr<Link>>> theBoard;
    int boardSize = 8;  // Board size for RAIInet
    bool gameOver = false; 
    Player players[2];
    int currentPlayer = 0;

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


            // Use a random device to generate a seed
            std::random_device rd;  
            std::mt19937 rng(rd()); // Standard mersenne_twister_engine seeded with rd()

            // Shuffle the vector
            std::shuffle(str.begin(), str.end(), rng);

            std::mt19937 rng(rd()); // Standard mersenne_twister_engine seeded with rd()

            // Shuffle the vector
            std::shuffle(types.begin(), types.end(), rng);

            for (int j = 0; j < 8; ++j) {
                if (i = 0) {
                    if (j = 3 || j = 4) {
                        l = Link(types[j], str[j], 1, j);
                    } else {
                        l = Link(types[j], str[j], 0, j);
                    }
                } else {
                    if (j = 3 || j = 4) {
                        l = Link(types[j], str[j], 7, j);
                    } else {
                        l = Link(types[j], str[j], 8, j);
                    }
                }
                players[i].addLink(l);
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
            theBoard[row][col] = std::make_unique<Link>(link);
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
        switch (dir) {
            case UP:    row -= link.getRange(); break;
            case DOWN:  row += link.getRange(); break;
            case LEFT:  col -= link.getRange(); break;
            case RIGHT: col += link.getRange(); break;
        }

        if (row >= 0 && row < boardSize && col >= 0 && col < boardSize) {
            // Move link to new position and clear old position
            theBoard[row][col] = std::move(theBoard[link.getRow()][link.getCol()]);
            link.setRow(row);
            link.setCol(col);
        } else {
            std::cerr << "Move out of bounds" << std::endl;
        }
    }

    // Check if a win/lose condition is met
    bool checkGameState() {
        // Implement game state checking logic
        return gameOver;
    }

    // Handle a battle between two links
    void handleLinkBattle(Link& attacker, Link& defender) {
        // Battle logic
    }

    // Getters for players
    Player& getPlayer(int playerNum) {
        return players[playerNum];
    }

    Player& getOpponent(int currentPlayer) {
        return players[1 - currentPlayer];
    }
};

// Overload the output operator to display the board
std::ostream &operator<<(std::ostream &out, const Board &board) {
    for (int i = 0; i < board.boardSize; ++i) {
        for (int j = 0; j < board.boardSize; ++j) {
            if (board.theBoard[i][j]) {
                out << board.theBoard[i][j]->getType() << ' ';
            } else {
                out << ". ";
            }
        }
        out << std::endl;
    }
    return out;
}

class Display {
    std::vector<std::vector<char>> theDisplay;
    std::unique_ptr<Board> board;

public:
    // Constructor
    Display(std::unique_ptr<Board> b) : board(std::move(b)) {
        theDisplay.resize(board->getBoardSize(), std::vector<char>(board->getBoardSize(), '.'));
    }

    // Update the internal representation of the board
    void updateDisplay() {
        // Clear the current display
        for (auto& row : theDisplay) {
            std::fill(row.begin(), row.end(), '.');
        }

        // Update the display based on the current state of the board
        for (int i = 0; i < board->getBoardSize(); ++i) {
            for (int j = 0; j < board->getBoardSize(); ++j) {
                auto& link = board->getLinkAt(i, j);
                if (link) {
                    theDisplay[i][j] = link->getType();
                }
            }
        }
    }

    // Output the current state of the display
    void render() const {
        for (const auto& row : theDisplay) {
            for (char cell : row) {
                std::cout << cell << ' ';
            }
            std::cout << '\n';
        }
    }

    // Friend declaration for output operator
    friend std::ostream &operator<<(std::ostream &out, const Display &display);
};

// Overload the output operator to display the board
std::ostream &operator<<(std::ostream &out, const Display &display) {
    display.render();
    return out;
}

#include <iostream>
#include <string>
#include <memory>
#include "Board.h"   // Assuming Board is defined in Board.h
#include "Display.h" // Assuming Display is defined in Display.h
#include "Link.h"    // Assuming Link is defined in Link.h

int main() {
    std::unique_ptr<Board> board = std::make_unique<Board>();
    Display display(std::move(board)); // Create a display for the board
    std::string cmd;
    int currentPlayer = 0;  // Start with player 0

    while (true) {
        std::cout << "Enter command: ";
        std::cin >> cmd;

        if (cmd == "new") {
            board->init();  // Initialize the board with players
            display.updateDisplay();
        } 
        else if (cmd == "move") {
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
        } 
        else if (cmd == "ability") {
            int ID;
            std::cin >> ID;
            board->getPlayer(currentPlayer).useAbility(ID); // Use an ability
            display.updateDisplay();
        } 
        else if (cmd == "status") {
            // Implement status display logic
            // This may involve displaying the number of downloaded links, remaining abilities, etc.
        } 
        else if (cmd == "quit") {
            std::cout << "Quitting the game." << std::endl;
            break; // Exit the game loop
        } 
        else {
            std::cerr << "Invalid command." << std::endl;
        }

        std::cout << display; // Display the board after each command
    }

class Observer {
 public:
  virtual void notify(Link *lp) = 0;
  virtual ~Observer() = default;
};

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

class Player {
    int playerNumber;
    std::vector<Link*> links;  // The Links that a Player owns
    int downloadedData = 0;  // the number of data that has been downloaded
    int downloadedVirus = 0;  // the number of virus that has been downloded
  public:
    Player(int playerNumber): playerNumber{playerNumber} {}

    int getData() const { return downloadedData; }
    int getViruses() const { return downloadedVirus; }
    Link* getLink(char letter) {
        for (auto& link : links) {
            if (link->getLetter() == letter) {
                return &link;
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
    void addLink(const Link link) {
        links.emplace_back(&link);
    }
};

class Board {
    std::vector<std::vector<Link*>> theBoard;
    int boardSize = 8;  // Board size for RAIInet
    bool Won = false;
    Player players[2];
    int currentPlayer = 0;
    Display *display = nullpter;

    void clearBoard();
public:
    Board() {}
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
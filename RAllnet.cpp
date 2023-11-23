class Link{
private:
    char type; // 'D' for data, 'V' for virus
    int strength; // Strength of the link [1,2,3,4]
    int row, col; // position
    bool state; // whether it is downloaded
    bool visible; // visibility of enemy player
public:
    Link(char type, int strength, int owner, bool visible);
    char getType() const;
    int getStrength() const;
    int getRow() const;
    int getCol() const;
    bool getState() const;
    bool getVisible() const;
    void toggleType();
    void setRow(int row);
    void setCol(int col);
    void toggleState();
    void toggleVisible();
    void interactWith(Link& otherLink); // New method for combat
};


class Ability { // Decorator
    bool state;
public:
    virtual void use() = 0;
    bool getState() const;
    void toggleState() const;
    virtual ~Ability() {}
};

class LinkBoost : public Ability {
    Link& link;
public:
    void use() override;
};

class Firewall : public Ability {
    Board board;
public:
    void use() override;
};

class Download : public Ability {
    Link& link;
    Board board;
public:
    void use() override;
};

class Polarize : public Ability {
    Link& link;
public:
    void use(Link& link) override;
};

class Scan : public Ability {
    Link& link;
    Board board;
public:
    void use() override;
};

class Player {
private:
    int playerNumber;
    std::vector<std::unique_ptr<Link>> links; // Player's links
    std::vector<std::unique_ptr<Ability>> abilities; // Player's abilities
    int downloadedData; // Count of downloaded data
    int downloadedViruses; // Count of downloaded viruses

    void downloadLink(Link& link); // Handle downloading a link
public:
    Player(int playerNumber);
    int getData() const;
    int getViruses() const;
    void moveLink(); // Manage player's links
    void useAbility(int ID); // Use an ability
    void addLink(const Link& link);
    void addAbility(const Ability& ability);
};




class Board { // grid
private:
    // Assuming an 8x8 board, based on the game description.
    std::vector<std::vector<Link>> theBorad;
    int boardSize;
    bool gameOver; // Flag to indicate if the game is over.
    Player players[2];
    int currentPlayer; // Track the current player (player 1 or player 2).
    Display *display;
    

public:
    Board();
    init(int n); // ctor
    void placeLink(const Link& link); // Place a link on the board
    enum Direction {UP, DOWN, LEFT, RIGHT};
    void moveLink(Link& link, Direction dir); // Move a link on the board
    bool checkGameState(); // Check if a win/lose condition is met
    void handleLinkBattle(Link& attacker, Link& defender);
    friend std::ostream &operator<<(std::ostream &out, const Board &board);
};

class Display { //textdisplay
private:
    std::vector<std::vector<char>> theDisplay;
    int gridSize;
    Board board;
public:
    Display(int n); //ctor
    void updateBoard(); // Update internal board representation
    void displayPlayerStats();
    friend std::ostream &operator<<(std::ostream &out, const Display &display);
};

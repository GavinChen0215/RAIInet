class Link {
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
    // Other necessary methods
};


class Ability { // Decorator
    bool state;
public:
    virtual void use() = 0;
    bool getState() const;
    virtual ~Ability() {}
};

class LinkBoost : public Ability {
public:
    void use() override;
};

class Firewall : public Ability {
public:
    void use() override;
};

class Download : public Ability {
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
public:
    void use(Link& link) override;
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
    std::vector<std::vector<Link*>> theBorad;
    int boardSize;
    bool gameOver; // Flag to indicate if the game is over.
    int currentPlayer; // Track the current player (player 1 or player 2).
    

public:
    Board();
    void placeLink(const Link& link); // Place a link on the board
    enum Direction {UP, DOWN, LEFT, RIGHT};
    void moveLink(Link& link, Direction dir); // Move a link on the board
    bool checkGameState(const Player& player); // Check if a win/lose condition is met
    void initializeBoard();
    void handleLinkBattle(Link& attacker, Link& defender);
};

class Display { //textdisplay
private:
    const Board& gameBoard; // Reference to the game board to display its state.
    Player &player;
    // For a graphical UI, you might have additional fields like:
    // Graphical components for rendering the board, links, etc.
    // Image assets or sprites for different link types.
public:
    Display();
    void updateBoard(const Board& board); // Update internal board representation
    void displayPlayerStats(const Player& player);
};

int main() {
    Board gameBoard;
    Player players[2];
    Display display;
    int currentPlayer;

    void processCommand(const std::string& command); // Process player commands
    void switchTurns(); // Switch turns between players
    bool checkWinCondition(); // Check if the game has been won
public:
    Game();
    void startGame(); // Initializes and starts the game loop
};

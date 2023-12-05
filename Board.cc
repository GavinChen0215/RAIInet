#include "Board.h"

using namespace std;

Board::Board(string links1, string links2, string abilities1, string abilities2) {
    for (int i = 0; i < boardSize; ++i) {
        vector<Square> row;
        for (int j = 0; j < boardSize; ++j) {
            Square s{i, j};
            row.emplace_back(s);
        }
        board.emplace_back(row);
    }
    players.emplace_back(make_shared<Player>(1, links1, abilities1));
    players.emplace_back(make_shared<Player>(2, links2, abilities2));
}

int Board::getCurrent() const { return currentPlayer; }

int Board::getWinner() const { return winner; }

bool Board::getIsOver() const { return isOver; }

void Board::setWinner(int playerNumber) { winner = playerNumber; }

void Board::toggleIsOver() { isOver = !isOver; }

void Board::switchPlayer() {
    currentPlayer = (currentPlayer == 2) ? 1 : 2;
}

void Board::updateGameState(int playerNumber) {
    if (players[playerNumber - 1]->getData() == 4) {
        toggleIsOver();
        setWinner(playerNumber);
    } else if (players[playerNumber - 1]->getViruses() == 4) {
        toggleIsOver();
        if (playerNumber == 1) {
            setWinner(playerNumber + 1);
        } else {
            setWinner(playerNumber - 1);
        }
    }
}

void Board::moveLink(char letter, Direction dir){
    int i = (currentPlayer == 1) ? (letter - 'a') : (letter - 'A');
    auto lp = players[currentPlayer - 1]->links[i];
    if (lp->getState()) {
        cout << "The link has been downloaded" << endl;
        cout << endl;
        return;
    }
    int row = lp->getRow();
    int col = lp->getCol();
    int newRow = row;
    int newCol = col;

    switch (dir) {
        case UP:    newRow -= lp->getRange(); break;
        case DOWN:  newRow += lp->getRange(); break;
        case LEFT:  newCol -= lp->getRange(); break;
        case RIGHT: newCol += lp->getRange(); break;
    }
    if ((newRow == 0 || newRow == 7) && (newCol == 3 || newCol == 4)) {  // move on to server port
        if ((currentPlayer == 1 && newRow == 7) || (currentPlayer == 2 && newRow == 0)) {
            downloadLink(letter);
        } else {
            cout << "You cannot move your link on top of your own server ports" << endl;
            return;
        }
    } else if (newRow >= 0 && newRow < boardSize && newCol >= 0 && newCol < boardSize) {  
        if (!(board[newRow][newCol].getLinkOn())) {   // move onto an empty square
            board[newRow][newCol].toggleLinkOn();
            board[newRow][newCol].setContent(letter);
        } else if (board[newRow][newCol].getLinkOn()) {  // move onto a square where there is already a Link
            char otherLetter = board[newRow][newCol].getContent();
            if (currentPlayer == 1 && (otherLetter >= 'a' && otherLetter <= 'h')) {
                cout << "You cannot move a link on top of another link owned by yourself" << endl;
                return;
            } else if (currentPlayer == 2 && (otherLetter >= 'A' && otherLetter <= 'H')) {
                cout << "You cannot move a link on top of another link owned by yourself" << endl;
                return;
            } else {  // [A~H] moves ontop of [a~h], battle
                if(battle(letter, otherLetter)) {
                    board[newRow][newCol].setContent(letter);
                }
            }
        }
    } else if ((newRow >= 8 && currentPlayer == 1) || (newRow <= -1 && currentPlayer == 2)) {  // move off from opponent's egde
        downloadLink(letter);
    } else {  // move off from other egdes => i.e. invalid move
        cout << "Invalid move! Out of boundary" << endl;
        return;
    }
    board[row][col].setContent('.');
    board[row][col].toggleLinkOn();
    lp->setRow(newRow);
    lp->setCol(newCol);
    switchPlayer();
}

bool Board::battle(char letter1, char letter2) {
    int i;
    int j;
    if (currentPlayer == 1) {
        i = letter1 - 'a';
        j = letter2 - 'A';
    } else {
        i = letter1 - 'A';
        j = letter2 - 'a';
    }
    int index = (currentPlayer == 1) ? 0 : 1;
    players[index]->links[i]->toggleVisbility();
    players[1 - index]->links[j]->toggleVisbility();
    int strength1 = players[index]->links[i]->getStrength();
    int strength2 = players[1 - index]->links[j]->getStrength();
    if (strength1 >= strength2) {
        downloadLink(letter2);
        return true;
    } else {
        downloadLink(letter1);
        return false;
    }
}

void Board::downloadLink(char letter) {
    int index;
    int i;
    if (letter >= 'A' && letter <= 'H') {
        index = letter - 'A';
        i = 0;
    } else {
        index = letter - 'a';
        i = 1;
    }
    players[1 - i]->links[index]->downloaded();
    if (players[1 - i]->links[index]->getType() == 'D') {
        players[i]->IncreData();
    } else {
        players[i]->IncreViruses();
    }
    updateGameState(i + 1);
}

bool Board::useAbility(int ID, istream& in) {
    int curr = getCurrent();
    if (players[curr - 1]->abilities[ID - 1]->getIsUsed()) {
        cout << "Invalid Ability: This Ability has been used... QwQ" << endl;
        return false;
    }
    AbilityName abName = players[curr - 1]->abilities[ID - 1]->getName();

    // LinkBoost
    if (abName == AbilityName::LinkBoost) {
        char linkLetter;
        if (in >> linkLetter) {
            // check if the Link belongs to the Player who used LinkBoost
            if (linkLetter >= 'a' && linkLetter <= 'h' && curr == 1) {
                // check if the Link represented by <linkLetter> still exists
                if (players[curr - 1]->links[linkLetter - 'a']->getState()) {
                    cout << "Invalid Link: This Link has been downloaded" << endl;
                    return false;
                } else {
                    // use ability
                    players[curr - 1]->links[linkLetter - 'a']->boostRange();
                    // update info
                    players[curr - 1]->abilities[ID - 1]->toggleIsUsed();
                    players[curr - 1]->DecreAbility();
                }
            } else if (linkLetter >= 'A' && linkLetter <= 'H' && curr == 2) {
                if (players[curr - 1]->links[linkLetter - 'A']->getState()) {
                    cout << "Invalid Link: This Link has been downloaded" << endl;
                    return false;
                } else {
                    players[curr - 1]->links[linkLetter - 'A']->boostRange();
                    players[curr - 1]->abilities[ID - 1]->toggleIsUsed();
                    players[curr - 1]->DecreAbility();
                }
            }
        }
    }

    // Download
    else if (abName == AbilityName::Download) {
        char oppoLink;
        if (in >> oppoLink) {
            // check if the Link represented by <oppoLink> actually belongs to the opponent
            if (curr == 1 && oppoLink >= 'A' && oppoLink <= 'H') {
                // check if the Link represented by <oppoLink> still exists
                if (players[1]->links[oppoLink - 'A']->getState()) {
                    cout << "Invalid Link: This Link has been downloaded" << endl;
                    return false;
                }
                auto theLink = players[1]->links[oppoLink - 'A'];
                int row = theLink->getRow();
                int col = theLink->getCol();
                theLink->toggleVisbility(); // don't need to reveal the Link downloaded using this Ability
                downloadLink(oppoLink);
                // update
                board[row][col].setContent('.');
                board[row][col].toggleLinkOn();
                players[curr - 1]->abilities[ID - 1]->toggleIsUsed();
                players[curr - 1]->DecreAbility();
            } else if (curr == 2 && oppoLink >= 'a' && oppoLink <= 'h') {
                if (players[0]->links[oppoLink - 'a']->getState()) {
                    cout << "Invalid Link: This Link has been downloaded" << endl;
                    return false;
                }
                auto theLink = players[0]->links[oppoLink - 'a'];
                int row = theLink->getRow();
                int col = theLink->getCol();
                downloadLink(oppoLink);
                // update
                players[0]->links[oppoLink - 'a']->toggleVisbility();
                board[row][col].setContent('.');
                board[row][col].toggleLinkOn();
                players[curr - 1]->abilities[ID - 1]->toggleIsUsed();
                players[curr - 1]->DecreAbility();
            } else {
                cout << "Invalid Link: Either doesn't exist or isn't owned by your opponenet" << endl;
                return false;
            }
        }
    } 
    
    // Polarize
    else if (abName == AbilityName::Polarize) {
        char linkLetter;
        if (in >> linkLetter) {
            // check if the Link is in the range of [a~h] or [A~H]
            if ((linkLetter < 'a' && linkLetter > 'h') && (linkLetter < 'A' && linkLetter > 'H')) {
                cout << "Invalid Link: This Link doesn't exist" << endl;
                return false;
            } else if (linkLetter >= 'a' && linkLetter <= 'h') {  // in range, yeah~ :3 
                // check if the Link represented by <linkLetter> has been downloaded
                if (players[0]->links[linkLetter - 'a']->getState()) {
                    cout << "Invalid Link: This Link has been downloaded" << endl;
                    return false;
                } else {
                    players[0]->links[linkLetter - 'a']->toggleType();
                    // update
                    players[curr - 1]->abilities[ID - 1]->toggleIsUsed();
                    players[curr - 1]->DecreAbility();
                    
                }
            } else if (linkLetter >= 'A' && linkLetter <= 'H' && curr == 2) {
                if (players[curr - 1]->links[linkLetter - 'A']->getState()) {
                    cout << "Invalid Link: This Link has been downloaded" << endl;
                    return false;
                } else {
                    players[1]->links[linkLetter - 'A']->toggleType();
                    // update
                    players[curr - 1]->abilities[ID - 1]->toggleIsUsed();
                    players[curr - 1]->DecreAbility();
                }
            }
        }
    }

    // Scan
    else if (abName == AbilityName::Scan) {
        char linkLetter;
        if (in >> linkLetter) {
            // check if the Link is in the range of [a~h] or [A~H]
            if ((linkLetter < 'a' && linkLetter > 'h') && (linkLetter < 'A' && linkLetter > 'H')) {
                cout << "Invalid Link: This Link doesn't exist" << endl;
                return false;
            } else if (linkLetter >= 'a' && linkLetter <= 'h') {
                // check if the Link represented by <linkLetter> has been downloaded
                if (players[0]->links[linkLetter - 'a']->getState()) {
                    cout << "Invalid Link: This Link has been downloaded" << endl;
                    return false;
                } else {
                    players[0]->links[linkLetter - 'a']->toggleVisbility();
                    // update
                    players[curr - 1]->abilities[ID - 1]->toggleIsUsed();
                    players[curr - 1]->DecreAbility();
                }
            } else if (linkLetter >= 'A' && linkLetter <= 'H') {
                if (players[curr - 1]->links[linkLetter - 'A']->getState()) {
                    cout << "Invalid Link: This Link has been downloaded" << endl;
                    return false;
                } else {
                    players[1]->links[linkLetter - 'A']->toggleVisbility();
                    // update
                    players[curr - 1]->abilities[ID - 1]->toggleIsUsed();
                    players[curr - 1]->DecreAbility();
                }
            }
        }
    }

    // Firewall
    else if (abName == AbilityName::Firewall) {
        char linkLetter;
        int row, col;
    }

    return true;
}

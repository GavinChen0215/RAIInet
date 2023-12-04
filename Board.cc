#include "Board.h"

using namespace std;

Board::Board(string links1, string links2) {
    for (int i = 0; i < boardSize; ++i) {
        vector<Square> row;
        for (int j = 0; j < boardSize; ++j) {
            Square s{i, j};
            row.emplace_back(s);
        }
        board.emplace_back(row);
    }
    players.emplace_back(make_shared<Player>(1, links1));
    players.emplace_back(make_shared<Player>(2, links2));
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
        cout << "The link is downloaded" << endl;
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
    if ((newRow == 0 || newRow == 7) && (newCol == 3 || newCol == 4)) {
        if ((currentPlayer == 1 && newRow == 7) || (currentPlayer == 2 && newRow == 0)) {
            downloadLink(letter);
        } else {
            cout << "You cannot move your link to you server spot" << endl;
            return;
        }
    } else if (newRow >= 0 && newRow < boardSize && newCol >= 0 && newCol < boardSize) {
        if (!(board[newRow][newCol].getLinkOn())) {
            board[newRow][newCol].toggleLinkOn();
            board[newRow][newCol].setContent(letter);
        } else {
            if(battle(letter, board[newRow][newCol].getContent())) {
                board[newRow][newCol].setContent(letter);
            }
        }
    } else {
        cout << "Move out of bounds" << endl;
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

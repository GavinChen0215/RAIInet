#include "Board.h"

using namespace std;

Board::Board(): players{Player(0), Player(1)} {}

int Board::getCurrentPlayer() { return currentPlayer; }

int Board::getBoardSize() { return boardSize; }

int Board::getWinner() { return winner; }

void Board::setWinner(int playerNumber) { winner = playerNumber; }

void Board::moveLink(Link& link, Direction dir){
    // FILL IN
} 

bool Board::isWon(){
    // FILL IN
}

void Board::Battle(Link& attakcer, Link& defender){
    // FILL IN
}

ostream &operator<<(ostream &out, const Board &board){
    // FILL IN
}


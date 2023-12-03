#include "Board.h"

using namespace std;

Board::Board(): players{Player(0), Player(1)} {}

void Board::updateBoard() {
    // FILL IN
}
void Board::placeLink(const Link& link) {
    // FILL IN
}

void Board::moveLink(Link& link, Direction dir){
    // FILL IN
} 

bool Board::checkGameState(){
    // FILL IN
}

void Board::handleLinkBattle(Link& attakcer, Link& defender){
    // FILL IN
}

Player Board::getPlayer(){
    // FILL IN
}

Player Board::getOpponent(){
    // FILL IN
}

ostream &operator<<(ostream &out, const Board &board){
    // FILL IN
}


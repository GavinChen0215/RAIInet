#include "Link.h"


using namespace std;

Link::Link(char type, int strength, int r, int c):
    type{type}, strength{strength}, row{r}, col{c} {
    // give the following fields default values
    range = 1;
    visibility = false;
    isDownload = false;
}

// accessor
char Link::getType() const { return type; } 
char Link::getStrength() const { return strength; }
int Link::getRow() const { return row; }
int Link::getCol() const { return col; }
int Link::getRange() const { return range; }
bool Link::getState() const { return isDownload; }
bool Link::getVisibility() const { return visibility; }

// mutator
void Link::toggleType() {
    (type == 'D') ? type = 'V' : type = 'D';  // change 'D' to 'V', 'V' to 'D'
}

void Link::boostRange() { range = 2; }  // change the range to 2, after using "LinkBoost"
void Link::setRow(int r) { row = r; }
void Link::setCol(int c) { col = c; }
void Link::toggleState() {
    isDownload ? false : true;  // negate the isDownload field
}
void Link::toggleVisbility() {
    visibility ? false : true;  // negate the visibility field
}

void Link::battleWith(Link &otherlink) {
    // TO BE FILLED IN...
}

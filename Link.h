#ifndef __LINK_H__
#define __LINK_H
#include <iostream>
#include <vector>

class Link {
    char type;  // 'D' for data, 'V' for virus
    int strength;  // strength of the Link, [1, 2, 3, 4]
    int row, col;  // position of the Link
    int range;  // how many grid can a Link move at a time
    bool visibility;  // whether the Link is visible to the opponent
    bool isDownload;  // whether the Link has been downloaded

  public:
    // constructor
    Link(char type, int strength, int r, int c, int range, bool visibility, bool isDowdload);
    // accessor
    char getType() const;
    char getStrength() const;
    int getRow() const;
    int getCol() const;
    int getRange() const;
    bool getState() const;  // return isDownload field
    bool getVisibility() const;
    // mutator
    void toggleType();
    void boostRange();  // for the purpose of the ability "LinkBoost"
    void setRow(int row);
    void setCol(int col);
    void toggleState();
    void toggleVisbility();
    // other methods
    void battleWith(Link &otherLink);  // method for the purpose of 'battle' between Links
};

#endif

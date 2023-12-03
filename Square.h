#ifndef __SQUARE__H__
#define __SQUARE__H__

// This class is used to represent each of the square (in total of 64) on the Board
class Square {
    int row, col;  // position
    char content;  // A~H, a~h, or a dot '.'
    bool linkOn = false;  // whether there is a Link placed on this Square
    bool isSP = false;  // whether this Square is a Server Port
  public:
    Square (int r, int c);  // ctor to give the position of the Square
};

#endif

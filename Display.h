#ifndef __DISPLAY__H__
#define __DISPLAY__H__

#include <vector>
#include "Board.h"

class Display {
    std::vector<std::vector<char>> theDisplay;

  public:
    Display();  // constructor
    void updateDisplayer();  // Update internal board representation
    friend std::ostream &operator<<(std::ostream &out, const Display &display);
};

#endif

#ifndef __GRAPHICSDISPLAY_H__
#define __GRAPHICSDISPLAY_H__
#include "window.h"
#include "link.h"

class GraphicsDisplay: public Observer {
    Xwindow &xw;
    int gridSize;
  public:
    GraphicsDisplay(Xwindow &xw, int size);
    void notify(link &l) override;
};

#endif

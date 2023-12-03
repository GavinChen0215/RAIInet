#include "graphicsdisplay.h"

GraphicsDisplay::GraphicsDisplay(Xwindow &xw, int size): xw{xw}, gridSize{size} {}
    
void GraphicsDisplay::notify(Cell &l) {
    const int windowSize = 500; // Default window size
    int cellSize = windowSize / gridSize;
    int col = c.getCol() * cellSize; // Size of every column
    int row = c.getRow() * cellSize; // Size of every row
    if (c.getState()) {
        // Fills white rectangles for the state of the cell is on
        xw.fillRectangle(col, row, cellSize, cellSize, Xwindow::White);
    } else {
        // Fills black rectangles for the state of the cell is off
        xw.fillRectangle(col, row, cellSize, cellSize, Xwindow::Black);
    }
}


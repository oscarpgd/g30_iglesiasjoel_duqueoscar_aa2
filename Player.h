#ifndef PLAYER_H
#define PLAYER_H

#include <Windows.h> 
#include "Types.h"

class Player {
private:
    int x, y;
    char symbol;

public:
    Player(int startX, int startY) : x(startX), y(startY), symbol('^') {} 

    int getX() const { return x; }
    int getY() const { return y; }
    char getSymbol() const { return symbol; }

    void update(char** matrix, int maxRows, int maxCols);
};

#endif
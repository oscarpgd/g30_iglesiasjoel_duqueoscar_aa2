#ifndef PLAYER_H
#define PLAYER_H

#include <Windows.h> 
#include "Types.h"
#include "People.h"

class Player {
private:
    int x, y;
    char symbol;
    int money;

public:
    Player(int startX, int startY) : x(startX), y(startY), symbol('^'), money(0) {}

    //Getes
    int GetX() const { return x; }
    int GetY() const { return y; }
    char GetSymbol() const { return symbol; }
    int GetMoney() { return money; }
    void AddMoney(int amount) { money += amount; }

    void Update(char** matrix, int maxRows, int maxCols, People** people, int numPeople);
};

#endif
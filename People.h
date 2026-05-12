#ifndef PEOPLE_H
#define PEOPLE_H

#include "Types.h"
#include <cstdlib>

class People {
private:
    Vec2 pos;
    bool isAlive;
    int isLand;

public:
    People(int startX, int startY, int islandID)
        : pos({ startX, startY }), isAlive(true), isLand(islandID) {
    }

    //Geters
    Vec2 GetPos() const { return pos; }
    bool GetIsAlive() const { return isAlive; }
    int GetIsland() const { return isLand; }

    void Update(char** matrix, int rows, int cols, Vec2 playerPos);
    void Die(char** matrix);
};

#endif

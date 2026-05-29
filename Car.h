#ifndef CAR_H
#define CAR_H

#include "Types.h"

class Car {
private:
    Vec2 pos;
    bool isOccupied;
    int islandID;

public:
    Car(int startX, int startY, int island)
        : pos({ startX, startY }), isOccupied(false), islandID(island) {
    }

    // Getters
    Vec2 GetPos() const { return pos; }
    bool IsOccupied() const { return isOccupied; }
    int GetIsland() const { return islandID; }

    void SetOccupied(bool state) { isOccupied = state; }
    void SetPos(int newX, int newY) { pos.x = newX; pos.y = newY; }
};

#endif
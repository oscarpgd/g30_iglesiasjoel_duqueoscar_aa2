#ifndef PLAYER_H
#define PLAYER_H

#include <Windows.h> 
#include "Types.h"
#include "People.h"
#include "Car.h"

class Player {
private:
    int x, y;
    char symbol;
    int money;
    bool isInCar;
    Car* currentCar;


public:
    Player(int startX, int startY) : x(startX), y(startY), symbol('^'), money(0), isInCar(false), currentCar(nullptr) {}

    //Getes
    int GetX() const { return x; }
    int GetY() const { return y; }
    char GetSymbol() const { return symbol; }
    int GetMoney() { return money; }
    bool IsInCar() { return isInCar; }
    Car* GetCurrentCar() const { return currentCar; }

    void AddMoney(int amount) { money += amount; }

    void EnterCar(Car* car);  
    void ExitCar();

    void Update(char** matrix, int maxRows, int maxCols, People** people, int numPeople, Car** cars, int numCars);
};

#endif
#ifndef PLAYER_H
#define PLAYER_H

#include <Windows.h> 
#include "Types.h"
#include "Car.h"

// Declaración adelantada para evitar dependencias circulares complejas
class People;

class Player {
private:
    int x, y;
    char symbol;
    int money;
    bool isInCar;
    Car* currentCar;

    int health;
    int maxHealth;
    int attackPower;

public:
    // Constructor
    Player(int startX, int startY, int startHealth, int startAttack)
        : x(startX), y(startY), symbol('^'), money(0), isInCar(false),
        currentCar(nullptr), health(startHealth), maxHealth(startHealth), attackPower(startAttack) {
    }

    // Getters y Setters
    int GetX() const { return x; }
    int GetY() const { return y; }
    char GetSymbol() const { return symbol; }
    int GetMoney() const { return money; }
    bool IsInCar() const { return isInCar; }
    Car* GetCurrentCar() const { return currentCar; }
    int GetHealth() const { return health; }
    int GetAttackPower() const { return attackPower; }

    void AddMoney(int amount) { money += amount; }
    void TakeDamage(int amount) { health -= amount; if (health < 0) health = 0; }

    void EnterCar(Car* car);
    void ExitCar();

    void Update(char** matrix, int maxRows, int maxCols, People** people, int numPeople, Car** cars, int numCars);
};

#endif
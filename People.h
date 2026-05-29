#ifndef PEOPLE_H
#define PEOPLE_H

#include "Types.h"
#include <cstdlib>

class People {
private:
    Vec2 pos;
    bool isAlive;
    int isLand;

    int health;
    int damage;
    bool isAggressive;
    bool isAlerted;
    float attackTimer; // Para controlar el ataque cada x segundos

    bool isBigSmoke; // el jefe tocho, es pa saber si un peaton lo inicializamos como jefe o no

public:

    People(int startX, int startY, int islandID, int startHealth, int startDamage, bool aggressive, bool bSmoke = false)
        : pos({ startX, startY }), isAlive(true), isLand(islandID),
        health(startHealth), damage(startDamage), isAggressive(aggressive),
        isAlerted(false), attackTimer(0.0f), isBigSmoke(bSmoke) {
    }

    // Getters y Setters
    Vec2 GetPos() const { return pos; }
    bool GetIsAlive() const { return isAlive; }
    int GetIsland() const { return isLand; }
    int GetHealth() const { return health; }
    bool IsAggressive() const { return isAggressive; }
    bool GetIsBigSmoke() const { return isBigSmoke; }

    // Métodos de juego
    void TakeDamage(int amount);
    void Update(char** matrix, int rows, int cols, Vec2 playerPos, float deltaTime, class Player& player);
    void Die(char** matrix);
};

#endif

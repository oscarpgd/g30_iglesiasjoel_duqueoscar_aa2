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

public:

    People(int startX, int startY, int islandID, int startHealth, int startDamage, bool aggressive)
        : pos({ startX, startY }), isAlive(true), isLand(islandID),
        health(startHealth), damage(startDamage), isAggressive(aggressive),
        isAlerted(false), attackTimer(0.0f) {
    }

    // Getters y Setters
    Vec2 GetPos() const { return pos; }
    bool GetIsAlive() const { return isAlive; }
    int GetIsland() const { return isLand; }
    int GetHealth() const { return health; }
    bool IsAggressive() const { return isAggressive; }

    // Métodos de juego
    void TakeDamage(int amount);
    void Update(char** matrix, int rows, int cols, Vec2 playerPos, float deltaTime, class Player& player);
    void Die(char** matrix);
};

#endif

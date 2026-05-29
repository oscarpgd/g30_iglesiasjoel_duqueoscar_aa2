#include "People.h"
#include <cmath>
#include <cstdlib>
#include "Player.h"

void People::TakeDamage(int amount) {
    if (!isAlive) return;

    health -= amount;
    // En cuanto le pega el primer golpe, si es agresivo, ataca
    if (isAggressive) {
        isAlerted = true;
    }

    if (health <= 0) {
        health = 0;
    }
}

void People::Update(char** matrix, int rows, int cols, Vec2 playerPos, float deltaTime, Player& player) {
    if (!isAlive) return;

    //Detección de proximidad, si el player está a 1 casilla, el peatón
    int distX = std::abs(pos.x - playerPos.x); // el abs es pa q sea simepre positivo
    int distY = std::abs(pos.y - playerPos.y);

    if (distX <= 1 && distY <= 1) {
        if (isAggressive && isAlerted) {
            attackTimer += deltaTime;
            if (attackTimer >= 1.0f) { // Ataca cada 1 segundo
                player.TakeDamage(damage);
                attackTimer = 0.0f;
            }
        }
        return; // No se mueve
    }

    // ia de movimiento
    // Dirección aleatoria 
        // 0 - arriba 
        // 1 - abajo
        // 2 - izquierda
        // 3 - derecha 
    int moveDir = rand() % 4;
    Vec2 nextPos = pos;

    if (moveDir == 0) nextPos.y--;
    else if (moveDir == 1) nextPos.y++;
    else if (moveDir == 2) nextPos.x--;
    else if (moveDir == 3) nextPos.x++;

    // Si no hay pared (X) se mueve
    if (nextPos.y >= 0 && nextPos.y < rows && nextPos.x >= 0 && nextPos.x < cols) {
        if (matrix[nextPos.y][nextPos.x] != 'X' && matrix[nextPos.y][nextPos.x] != 'T') {
            pos = nextPos;
        }
    }
}

void People::Die(char** matrix)
{
    isAlive = false;
    matrix[pos.y][pos.x] = '$'; //Deja dinero en el suelo 
}

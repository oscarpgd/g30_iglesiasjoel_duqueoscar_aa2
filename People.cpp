#include "People.h"
#include <cmath>
#include <cstdlib>

void People::Update(char** matrix, int rows, int cols, Vec2 playerPos) {
    if (!isAlive) return;

    //Detección de proximidad, si el player está a 1 casilla, el peatón
    //Conversion a abs para que las comparaciones sean siempre en positivo y mas faciles
    int distX = std::abs(pos.x - playerPos.x);
    int distY = std::abs(pos.y - playerPos.y);

    if (distX <= 1 && distY <= 1) {
        return;  
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
        if (matrix[nextPos.y][nextPos.x] != 'X') {
            pos = nextPos;
        }
    }
}

void People::Die(char** matrix)
{
    isAlive = false;
    matrix[pos.y][pos.x] = '$'; //Deja dinero en el suelo 
}

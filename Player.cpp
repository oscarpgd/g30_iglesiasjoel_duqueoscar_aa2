#include "Player.h"

void Player::update(char** matrix, int maxRows, int maxCols) {
    int nextX = x;
    int nextY = y;

    if (GetAsyncKeyState(VK_UP)) {
        nextY--;
        symbol = '^'; // Arriba
    }
    else if (GetAsyncKeyState(VK_DOWN)) {
        nextY++;
        symbol = 'v'; // Abajo
    }
    else if (GetAsyncKeyState(VK_LEFT)) {
        nextX--;
        symbol = '<'; //Izq
    }
    else if (GetAsyncKeyState(VK_RIGHT)) {
        nextX++;
        symbol = '>'; // Derecha
    }

    // Colision
    if (nextY >= 0 && nextY < maxRows && nextX >= 0 && nextX < maxCols) {
        if (matrix[nextY][nextX] != 'X') {
            x = nextX;
            y = nextY;
        }
    }
}
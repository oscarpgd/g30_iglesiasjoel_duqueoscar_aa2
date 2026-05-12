#include "Player.h"

void Player::Update(char** matrix, int maxRows, int maxCols, People** people, int numPeople) {
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
    if (nextY >= 0 && nextY < maxRows && nextX >= 0 && nextX < maxCols) 
    {
        //Comprobaciónes de pared
        if (matrix[nextY][nextX] != 'X') 
        {
            //Comprobaciones de paton
            bool occupiedByPed = false;
            for (int i = 0; i < numPeople; i++) {
                if (people[i]->GetIsAlive() &&
                    people[i]->GetPos().x == nextX &&
                    people[i]->GetPos().y == nextY) {
                    occupiedByPed = true;
                    break;
                }
            }

            if (!occupiedByPed) {
                x = nextX;
                y = nextY;
            }
        }
    }
}
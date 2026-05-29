#include "Player.h"

void Player::Update(char** matrix, int maxRows, int maxCols, People** people, int numPeople, Car** cars, int numCars) {
    if (GetAsyncKeyState('E') & 0x8000) {
        if (isInCar) {
            ExitCar();
        }
        else {
            // Busca coche adyacente
            for (int i = 0; i < numCars; i++) {
                Vec2 cPos = cars[i]->GetPos();
                if (!cars[i]->IsOccupied() &&
                    abs(cPos.x - x) <= 1 &&
                    abs(cPos.y - y) <= 1) {
                    EnterCar(cars[i]);
                    break;
                }
            }
        }
    }


    int nextX = x;
    int nextY = y;

    if (!IsInCar()) {
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
    }
    else {
        if (GetAsyncKeyState(VK_UP)) {
            nextY--;
            symbol = 'C'; // Arriba
        }
        else if (GetAsyncKeyState(VK_DOWN)) {
            nextY++;
            symbol = 'C'; // Abajo
        }
        else if (GetAsyncKeyState(VK_LEFT)) {
            nextX--;
            symbol = 'C'; //Izq
        }
        else if (GetAsyncKeyState(VK_RIGHT)) {
            nextX++;
            symbol = 'C'; // Derecha
        }
    }
    

    // Colision
    if (nextY >= 0 && nextY < maxRows && nextX >= 0 && nextX < maxCols) 
    {
        //Comprobaciónes de pared
        if (matrix[nextY][nextX] != 'X' || matrix[nextY][nextX] != 'o')
        {
            if (this->IsInCar() && matrix[nextY][nextX] == '$') {
                return;
            }

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

            //Comprobación de Coche
            bool occupiedByCar = false;
            {
                for (int i = 0; i < numCars; i++) 
                {
                    if (cars[i]->IsOccupied() && people[i]->GetPos().x == nextX && people[i]->GetPos().y == nextY) 
                    {
                        occupiedByCar = true;
                        break;
                    }
                }
            }
        }
    }
}

void Player::EnterCar(Car* car)
{
    isInCar = true;
    currentCar = car;
    car->SetOccupied(true);
    symbol = 'C';
}

void Player::ExitCar() {
    if (currentCar != nullptr) {
        currentCar->SetOccupied(false);
        currentCar = nullptr;
    }
    isInCar = false;
    symbol = '^';
}

#include "Player.h"
#include "People.h"
#include <cmath>

void Player::Update(char** matrix, int maxRows, int maxCols, People** people, int numPeople, Car** cars, int numCars) {
    //Subir bajar coche
    if (GetAsyncKeyState('E') & 0x8000) {
        if (isInCar) {
            bool safeExitFound = false;
            for (int dy = -1; dy <= 1 && !safeExitFound; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    int checkX = x + dx;
                    int checkY = y + dy;
                    if (checkY >= 0 && checkY < maxRows && checkX >= 0 && checkX < maxCols) {
                        
                        if (matrix[checkY][checkX] != 'X' && matrix[checkY][checkX] != 'T') {
                            
                            bool pedThere = false;
                            for (int p = 0; p < numPeople; p++) {
                                if (people[p]->GetIsAlive() && people[p]->GetPos().x == checkX && people[p]->GetPos().y == checkY) {
                                    pedThere = true;
                                    break;
                                }
                            }
                            if (!pedThere) {
                                x = checkX;
                                y = checkY;
                                ExitCar();
                                safeExitFound = true;
                                break;
                            }
                        }
                    }
                }
            }
            Sleep(200); //anti bugs
        }
        else {
            // Buscar coche adyacente
            for (int i = 0; i < numCars; i++) {
                Vec2 cPos = cars[i]->GetPos();
                if (!cars[i]->IsOccupied() && abs(cPos.x - x) <= 1 && abs(cPos.y - y) <= 1) {
                    EnterCar(cars[i]);
                    // Al subir, nos posicionamos exactamente donde está el coche
                    x = cPos.x;
                    y = cPos.y;
                    break;
                }
            }
            Sleep(200);
        }
    }

    int nextX = x;
    int nextY = y;
    char nextSymbol = symbol;

    if (GetAsyncKeyState(VK_UP)) {
        nextY--;
        nextSymbol = isInCar ? 'C' : '^';
    }
    else if (GetAsyncKeyState(VK_DOWN)) {
        nextY++;
        nextSymbol = isInCar ? 'C' : 'v';
    }
    else if (GetAsyncKeyState(VK_LEFT)) {
        nextX--;
        nextSymbol = isInCar ? 'C' : '<';
    }
    else if (GetAsyncKeyState(VK_RIGHT)) {
        nextX++;
        nextSymbol = isInCar ? 'C' : '>';
    }

    // colisiones
    if (nextY >= 0 && nextY < maxRows && nextX >= 0 && nextX < maxCols) {

        if (matrix[nextY][nextX] != 'X') {

            if (isInCar && matrix[nextY][nextX] == '$') {
                return;
            }

            bool occupiedByPed = false;
            for (int i = 0; i < numPeople; i++) {
                if (people[i]->GetIsAlive() && people[i]->GetPos().x == nextX && people[i]->GetPos().y == nextY) {
                    occupiedByPed = true;
                    break;
                }
            }

            if (occupiedByPed && !isInCar) {
                return;
            }

            bool occupiedByCar = false;
            for (int i = 0; i < numCars; i++) {
                if (cars[i] != currentCar && cars[i]->GetPos().x == nextX && cars[i]->GetPos().y == nextY) {
                    occupiedByCar = true;
                    break;
                }
            }

            if (!occupiedByCar) {
                x = nextX;
                y = nextY;
                symbol = nextSymbol;

                if (isInCar && currentCar != nullptr) {
                    currentCar->SetPos(x, y);
                }
            }
        }
    }
}

void Player::EnterCar(Car* car) {
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
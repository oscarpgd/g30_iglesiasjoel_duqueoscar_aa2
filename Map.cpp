#include "Map.h"
#include <Windows.h>

Map::Map() : matrix(nullptr), rows(0), cols(0), toll_onePaid(false), toll_twoPaid(false) {}

Map::~Map() {
    if (matrix != nullptr) {
        for (int i = 0; i < rows; ++i) {
            delete[] matrix[i];
        }
        delete[] matrix;
    }
}

bool Map::LoadConfig(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    std::string val;

    if (!std::getline(file, val, ';')) return false; cols = std::stoi(val);
    if (!std::getline(file, val, ';')) return false; rows = std::stoi(val);

    // Linea 2: stats de CJ (No las leías antes, ahora sí para inicializar el Player)
    if (!std::getline(file, val, ';')) return false; gameConfig.cjMaxHealth = std::stoi(val);
    if (!std::getline(file, val, ';')) return false; gameConfig.cjAttack = std::stoi(val);

    // Linea 3: precios de peaje
    if (!std::getline(file, val, ';')) return false; gameConfig.toll_onePrice = std::stoi(val);
    if (!std::getline(file, val, ';')) return false; gameConfig.toll_twoPrice = std::stoi(val);

    // Linea 4: info isla 1 (Los Santos)
    if (!std::getline(file, val, ';')) return false; islands[0].numPeople = std::stoi(val);
    if (!std::getline(file, val, ';')) return false; islands[0].maxMoneyPerPeo = std::stoi(val);
    if (!std::getline(file, val, ';')) return false; islands[0].pedestrianHealth = std::stoi(val);
    if (!std::getline(file, val, ';')) return false; islands[0].pedestrianAttack = std::stoi(val);

    // Linea 5: info isla 2 (San Fierro)
    if (!std::getline(file, val, ';')) return false; islands[1].numPeople = std::stoi(val);
    if (!std::getline(file, val, ';')) return false; islands[1].maxMoneyPerPeo = std::stoi(val);
    if (!std::getline(file, val, ';')) return false; islands[1].pedestrianHealth = std::stoi(val);
    if (!std::getline(file, val, ';')) return false; islands[1].pedestrianAttack = std::stoi(val);

    // Linea 6: info isla 3 (Las Venturas)
    if (!std::getline(file, val, ';')) return false; islands[2].numPeople = std::stoi(val);
    if (!std::getline(file, val, ';')) return false; islands[2].maxMoneyPerPeo = std::stoi(val);
    if (!std::getline(file, val, ';')) return false; islands[2].pedestrianHealth = std::stoi(val);
    if (!std::getline(file, val, ';')) return false; islands[2].pedestrianAttack = std::stoi(val);

    file.close();
    InitMatrix();
    return true;
}

void Map::InitMatrix() {
    // Hacemos un x3 porque en ancho son 3 islas puestas al lado, en largo da igual porque abajo no hay
    int realCols = (cols * 3) + 2;
   
    matrix = new char* [rows];
    for (int i = 0; i < rows; ++i) {
        matrix[i] = new char[realCols];
        for (int j = 0; j < realCols; ++j) {
            // Paredes exteriores 
            if (i == 0 || i == rows - 1 || j == 0 || j == realCols - 1) {
                matrix[i][j] = 'X'; 
            }
            else {
                matrix[i][j] = ' ';
            }
        }
    }
    
    int wall1 = cols;      // Fin de Los Santos
    int wall2 = cols * 2 + 1; // Fin de San Fierro

    for (int i = 0; i < rows; ++i) {
        matrix[i][wall1] = 'X'; 
        matrix[i][wall2] = 'X'; 
    }

    // Puentes
    int bridgePos = rows / 2;
    matrix[bridgePos][wall1] = 'T';
    matrix[bridgePos][wall2] = 'T';

        
    this->cols = realCols;
}

void Map::PrintFullMap() 
{
    if (matrix == nullptr) return;

    //doble bucle para x e y
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cout << matrix[i][j];
        }
        std::cout << std::endl;
    }
}

void Map::PrintView(int playerX, int playerY, char playerSymbol, People** peds, int numPeds, Player player, Car** cars, int numCars) {
    int radiusY = 8;
    int radiusX = 15;

    int startY = playerY - radiusY;
    int endY = playerY + radiusY;
    int startX = playerX - radiusX;
    int endX = playerX + radiusX;

    for (int i = startY; i <= endY; ++i) {
        for (int j = startX; j <= endX; ++j) {
            bool carPrinted = false;
            for (int c = 0; c < numCars; c++) {
                if (!cars[c]->IsOccupied() && cars[c]->GetPos().y == i && cars[c]->GetPos().x == j) {
                    std::cout << 'C';
                    carPrinted = true;
                    break;
                }
            }
            if (carPrinted) continue;

            if (i == playerY && j == playerX) {
                std::cout << playerSymbol;
            }
            else {
                bool pedPrinted = false;
                for (int p = 0; p < numPeds; p++) {
                    if (peds[p]->GetIsAlive() && peds[p]->GetPos().y == i && peds[p]->GetPos().x == j) {
                        std::cout << 'P';
                        pedPrinted = true;
                        break;
                    }
                }

                if (!pedPrinted) {
                    if (i >= 0 && i < rows && j >= 0 && j < cols) {
                        std::cout << matrix[i][j];
                    }
                    else {
                        std::cout << ' ';
                    }
                }
            }
        }
        std::cout << "\n";
    }
    std::cout << "\nVida CJ: [" << player.GetHealth() << "] | Dinero CJ: [" << player.GetMoney() << "] | Pulsa ESC para salir" << std::endl;
}

void Map::UnlockToll_one() {
    toll_onePaid = true;
    int bridgePos = rows / 2;
    int wall1 = cols / 3;      
    matrix[bridgePos][wall1] = ' ';
}

void Map::UnlockToll_two() {
    toll_twoPaid = true;
    int bridgePos = rows / 2;
    int wall2 = (cols / 3) * 2 + 1;
    matrix[bridgePos][wall2] = ' ';
}
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

    // liena 1 dimensiones
    std::getline(file, val, ';'); cols = std::stoi(val);
    std::getline(file, val, ';'); rows = std::stoi(val);

    //linea 2 (precios de peaje) cambiar más adelante
    std::getline(file, val, ';'); gameConfig.toll_onePrice = std::stoi(val);
    std::getline(file, val, ';'); gameConfig.toll_twoPrice = std::stoi(val);

    //info isla 1
    std::getline(file, val, ';'); islands[0].numPeople = std::stoi(val);
    std::getline(file, val, ';'); islands[0].price = std::stoi(val);
    std::getline(file, val, ';'); islands[0].maxMoneyPerPeo = std::stoi(val);

    //info isla 2
    std::getline(file, val, ';'); islands[1].numPeople = std::stoi(val);
    std::getline(file, val, ';'); islands[1].price = std::stoi(val);
    std::getline(file, val, ';'); islands[1].maxMoneyPerPeo = std::stoi(val);
    //USo de stoi para convertir string a int




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
    matrix[bridgePos][wall1] = 'o';
    matrix[bridgePos][wall2] = 'o'; 

        
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

void Map::PrintView(int playerX, int playerY, char playerSymbol, People** peds, int numPeds, Player player, Car** cars, int numCars) 
{
    int radiusY = 8;  // Alto de la vista
    int radiusX = 15; // Ancho de la vista


    int startY = playerY - radiusY;
    int endY = playerY + radiusY;
    int startX = playerX - radiusX;
    int endX = playerX + radiusX;

    for (int i = startY; i <= endY; ++i) 
    {
        for (int j = startX; j <= endX; ++j) 
        {
            //Coches
            bool carPrinted = false;
            for (int c = 0; c < numCars; c++) {
                if (!cars[c]->IsOccupied() && cars[c]->GetPos().y == i && cars[c]->GetPos().x == j)
                {
                    std::cout << 'C';
                    carPrinted = true;
                    break;
                }
            }
            if (carPrinted) continue;

            //PEatones
            if (i == playerY && j == playerX)
            {
                std::cout << playerSymbol;
            }
            else {

                bool pedPrinted = false;
                for (int p = 0; p < numPeds; p++) 
                {
                    if (peds[p]->GetIsAlive() &&
                        peds[p]->GetPos().y == i &&
                        peds[p]->GetPos().x == j) 
                    {
                        std::cout << 'P';
                        pedPrinted = true;
                        break;
                    }
                }

                if (!pedPrinted) 
                {
                    if (i >= 0 && i < rows && j >= 0 && j < cols) 
                    {
                        std::cout << matrix[i][j];
                    }
                    else
                    {
                        std::cout << ' ';
                    }
                }
            }
        }
        std::cout << "\n";
    }
    std::cout << "\nDinero CJ: [" << player.GetMoney() << "] | Pulsa ESC para salir" << std::endl;
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
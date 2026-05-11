#include "Map.h"
#include <Windows.h>

Map::Map() : matrix(nullptr), rows(0), cols(0) {}

Map::~Map() {
    if (matrix != nullptr) {
        for (int i = 0; i < rows; ++i) {
            delete[] matrix[i];
        }
        delete[] matrix;
    }
}

bool Map::loadConfig(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    std::string value;
    // Lee ancho y alto
    if (std::getline(file, value, ';')) cols = std::stoi(value);
    if (std::getline(file, value, ';')) rows = std::stoi(value);

    file.close();
    initMatrix();
    return true;
}

void Map::initMatrix() {
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
    matrix[bridgePos][wall1] = ' ';
        matrix[bridgePos][wall2] = ' '; 

        
        this->cols = realCols;
}

void Map::printFullMap() {
    if (matrix == nullptr) return;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cout << matrix[i][j];
        }
        std::cout << std::endl;
    }
}

void Map::printView(int playerX, int playerY, char playerSymbol) {

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = { 0, 0 };
    SetConsoleCursorPosition(hOut, coord);

    
    int viewRadiusY = 10; // Alto de la vista
    int viewRadiusX = 20; // Ancho de la vista, es mas que la Y porq la consola no es cuadrada, es rectangular

    int startY = playerY - viewRadiusY;
    int endY = playerY + viewRadiusY;
    int startX = playerX - viewRadiusX;
    int endX = playerX + viewRadiusX;

    for (int i = startY; i <= endY; ++i) {
        for (int j = startX; j <= endX; ++j) {
            // Posición del jugadorr 
            if (i == playerY && j == playerX) {
                std::cout << playerSymbol;
            }
            // Dentro
            else if (i >= 0 && i < rows && j >= 0 && j < cols) {
                std::cout << matrix[i][j];
            }
            // Fuera
            else {
                std::cout << ' ';
            }
        }
        std::cout << "\n";
    }

    // debug
    std::cout << "\nPosicion: [" << playerX << ", " << playerY << "]\n";
}
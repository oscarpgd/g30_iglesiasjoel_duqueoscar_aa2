#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <fstream>
#include <string>

class Map {
private:
    char** matrix;     
    int rows;           
    int cols;           

public:
    Map();
    ~Map(); // Esta es para liberar memoriaa

    bool loadConfig(const std::string& filename);

    void initMatrix();

    void printFullMap();

    int getRows() const { return rows; }
    int getCols() const { return cols; }
    char** getMatrix() { return matrix; }


    void printView(int playerX, int playerY, char playerSymbol);
    //static const int VIEW_SIZE = 10; // Radio de visión


    

    
};

#endif
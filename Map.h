#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <fstream>
#include <string>
#include "People.h"
#include "Player.h"

class Map {
private:
    char** matrix;     
    int rows;           
    int cols;
    IslandConfig islands[2];

public:
    Map();
    ~Map();

    bool LoadConfig(const std::string& filename);

    void InitMatrix();

    void PrintFullMap();

    //Geters
    int GetRows() const { return rows; }
    int GetCols() const { return cols; }
    char** GetMatrix() { return matrix; }
    IslandConfig GetIslandData(int index) const { return islands[index]; }

    void PrintView(int playerX, int playerY, char playerSymbol, People** peds, int numPeds, Player player);
};

#endif
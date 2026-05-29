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
    IslandConfig islands[3]; 
    int numOfCars;
    GameConfig gameConfig;
    bool toll_onePaid;
    bool toll_twoPaid;

public:
    Map();
    ~Map();

    bool LoadConfig(const std::string& filename);
    void InitMatrix();
    void PrintFullMap();

    // Getters básicos
    int GetRows() const { return rows; }
    int GetCols() const { return cols; }
    char** GetMatrix() { return matrix; }
    IslandConfig GetIslandData(int index) const { return islands[index]; }
    int GetCars() { return numOfCars; }
    int GetCjMaxHealth() const { return gameConfig.cjMaxHealth; }
    int GetCjAttack() const { return gameConfig.cjAttack; }

    // Peajes
    void UnlockToll_one();
    void UnlockToll_two();

    int GetTollOnePrice() const { return gameConfig.toll_onePrice; }
    int GetTollTwoPrice() const { return gameConfig.toll_twoPrice; }
    bool IsTollOnePaid() const { return toll_onePaid; }
    bool IsTollTwoPaid() const { return toll_twoPaid; }

    void PrintView(int playerX, int playerY, char playerSymbol, People** peds, int numPeds, Player player, Car** cars, int numCars);
};

#endif
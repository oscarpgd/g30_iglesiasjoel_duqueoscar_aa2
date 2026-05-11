#ifndef TYPES_H
#define TYPES_H

//coordenadas
struct Vec2 {
    int x, y;
};

// Guarda lo speatones, peaje, dinero máximo de cada isla
struct IslandConfig {
    int numPeople;
    int price;
    int maxMoneyPerPeo;
};

// Guarda las dimensions del mapa
struct GameConfig {
    int width, height;
    IslandConfig islands[2];
};

// Tipos de direcciones
enum class Direction { UP, DOWN, LEFT, RIGHT };

#endif
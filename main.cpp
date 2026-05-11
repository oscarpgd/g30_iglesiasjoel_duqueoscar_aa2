#include <iostream>
#include <Windows.h>
#include "Map.h"
#include "Player.h"

int main() {
    Map gameMap;

    // Aqui cargamos el archivo de config
    if (!gameMap.loadConfig("config.txt")) {
        std::cerr << "No se encuentra" << std::endl;
        return 1;
    }

    // Le damos la posi al player
    Player cj(5, 5);

    while (true) {
        
        if (GetAsyncKeyState(VK_ESCAPE)) break;

        // Actualiza la posicion de CJ (pasa la matriz para detectar colisiones con la pareds)
        cj.update(gameMap.getMatrix(), gameMap.getRows(), gameMap.getCols());

        // Dibujar el mapa que vemos
        gameMap.printView(cj.getX(), cj.getY(), cj.getSymbol());

        
        Sleep(100);
    }

    return 0;
}
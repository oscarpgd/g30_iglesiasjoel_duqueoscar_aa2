#include <iostream>
#include <Windows.h>
#include <ctime>
#include "Map.h"
#include "Player.h"
#include "People.h"
#include "Car.h"


int main() {
    // INICIALIZACIÓN
    srand((time(NULL)));

    Map gameMap;

    // Lectura del archivo
    if (!gameMap.LoadConfig("config.txt")) {
        std::cerr << "Error: No se pudo cargar config.txt" << std::endl;
        return 1;
    }

    // peatones
    int nLosSantos = gameMap.GetIslandData(0).numPeople;
    int nSanFierro = gameMap.GetIslandData(1).numPeople;
    int nLasVenturas = gameMap.GetIslandData(2).numPeople;

    int totalPeople = nLosSantos + nSanFierro + nLasVenturas + 1;
    People** people = new People * [totalPeople];

    // Cochces
    const int carsInLosSantos = 3;
    const int carsInSanFierro = 2;
    const int carsInLasVenturas = 4;
    int totalCars = carsInLosSantos + carsInSanFierro + carsInLasVenturas;
    Car** cars = new Car * [totalCars];
    int carIndex = 0;

    int islandWidth = gameMap.GetCols() / 3;
    int pIndex = 0;

    // Spawn de peatones - Los Santos (Isla 0)
    for (int i = 0; i < nLosSantos; i++) {
        int rx = rand() % (islandWidth - 2) + 1;
        int ry = rand() % (gameMap.GetRows() - 2) + 1;
        bool isAggressive = (rand() % 2) == 0;
        IslandConfig data = gameMap.GetIslandData(0);
        people[pIndex++] = new People(rx, ry, 0, data.pedestrianHealth, data.pedestrianAttack, isAggressive);
    }

    // Spawn de peatones - Los Santos (Isla 1)
    for (int i = 0; i < nSanFierro; i++) {
        int rx = rand() % (islandWidth - 2) + islandWidth + 1;
        int ry = rand() % (gameMap.GetRows() - 2) + 1;
        bool isAggressive = (rand() % 2) == 0;
        IslandConfig data = gameMap.GetIslandData(1);
        people[pIndex++] = new People(rx, ry, 1, data.pedestrianHealth, data.pedestrianAttack, isAggressive);
    }

    // Spawn de peatones, solo los NORMALES - Los Santos (Isla 2)
    for (int i = 0; i < nLasVenturas; i++) {
        int rx = rand() % (islandWidth - 2) + (islandWidth * 2) + 2;
        int ry = rand() % (gameMap.GetRows() - 2) + 1;
        bool isAggressive = (rand() % 2) == 0;
        IslandConfig data = gameMap.GetIslandData(2);
        people[pIndex++] = new People(rx, ry, 2, data.pedestrianHealth, data.pedestrianAttack, isAggressive);
    }

    // Spawn de Big Smoke
    int bsX = rand() % (islandWidth - 2) + (islandWidth * 2) + 2;
    int bsY = rand() % (gameMap.GetRows() - 2) + 1;
    // Stats bufadas
    people[pIndex++] = new People(bsX, bsY, 2, 500, 40, true, true);

    // Coches pa cada isla
    for (int i = 0; i < carsInLosSantos; i++) {
        int rx = rand() % (islandWidth - 2) + 1;
        int ry = rand() % (gameMap.GetRows() - 2) + 1;
        cars[carIndex++] = new Car(rx, ry, 0);
    }
    for (int i = 0; i < carsInSanFierro; i++) {
        int rx = rand() % (islandWidth - 2) + islandWidth + 1;
        int ry = rand() % (gameMap.GetRows() - 2) + 1;
        cars[carIndex++] = new Car(rx, ry, 1);
    }
    for (int i = 0; i < carsInLasVenturas; i++) {
        int rx = rand() % (islandWidth - 2) + islandWidth * 2 + 2;
        int ry = rand() % (gameMap.GetRows() - 2) + 1;
        cars[carIndex++] = new Car(rx, ry, 2);
    }

    Player cj(5, 5, gameMap.GetCjMaxHealth(), gameMap.GetCjAttack());
    float deltaTime = 0.1f;

    // Variables de control de la maquina de estados y menú
    GameState currentState = GameState::INIT;
    int menuOption = 0; // 0 = Play, 1 = Exit
    std::string gameOverMessage = "";

    // BUCLE PRINCIPAL
    while (true) {
        system("cls");

        switch (currentState) {

        case GameState::INIT: {
            // Estado Splash Screen: Muestra el título durante 3 segundos y pasa al menú
            std::cout << "\n\n\n" << std::endl;
            std::cout << "  ###############################################" << std::endl;
            std::cout << "  #                                             #" << std::endl;
            std::cout << "  #         GRAND THEFT AUTO: ENTI CITY         #" << std::endl;
            std::cout << "  #                                             #" << std::endl;
            std::cout << "  ###############################################" << std::endl;
            Sleep(3000);
            currentState = GameState::MENU;
            break;
        }

        case GameState::MENU: {
            // Estado Menú Principal q se puede clikar
            std::cout << "=========================================" << std::endl;
            std::cout << "             MENU PRINCIPAL              " << std::endl;
            std::cout << "=========================================\n" << std::endl;

            if (menuOption == 0) std::cout << "  -> [ PLAY ]" << std::endl;
            else std::cout << "     [ PLAY ]" << std::endl;

            if (menuOption == 1) std::cout << "  -> [ EXIT ]" << std::endl;
            else std::cout << "     [ EXIT ]" << std::endl;

            std::cout << "\n=========================================" << std::endl;
            std::cout << " Usa Flecha Arriba/Abajo y ESPACIO para seleccionar" << std::endl;

            // inputs del menu
            if (GetAsyncKeyState(VK_UP) & 0x8000) {
                menuOption = 0;
                Sleep(150);
            }
            else if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
                menuOption = 1;
                Sleep(150);
            }
            else if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
                if (menuOption == 0) {
                    currentState = GameState::GAME;
                }
                else {
                    // Selección de Exit: Limpieza de memoria y cierre 
                    goto cleanup;
                }
                Sleep(200);
            }
            Sleep(50);
            break;
        }

        case GameState::GAME: {
            // Estado principal del juego en ejecución
            if (GetAsyncKeyState(VK_ESCAPE)) {
                currentState = GameState::MENU;
                Sleep(200);
                break;
            }

            //derrota por salud
            if (cj.GetHealth() <= 0) {
                gameOverMessage = "¡CJ SE HA QUEDADO SIN VIDA!";
                currentState = GameState::GAME_OVER;
                break;
            }

            cj.Update(gameMap.GetMatrix(), gameMap.GetRows(), gameMap.GetCols(), people, totalPeople, cars, totalCars);
            Vec2 cjPos = { cj.GetX(), cj.GetY() };

            for (int i = 0; i < totalPeople; i++) {
                people[i]->Update(gameMap.GetMatrix(), gameMap.GetRows(), gameMap.GetCols(), cjPos, deltaTime, cj);
            }

            // Atropello automático si vas en coche
            for (int i = 0; i < totalPeople; i++) {
                if (cj.IsInCar() && people[i]->GetIsAlive()) {
                    Vec2 pPos = people[i]->GetPos();
                    if (abs(pPos.x - cj.GetX()) <= 1 && abs(pPos.y - cj.GetY()) <= 1) {

                        if (people[i]->GetIsBigSmoke()) continue;

                        people[i]->Die(gameMap.GetMatrix());
                        int island = people[i]->GetIsland();
                        int offset = (island == 0) ? 0 : (island == 1) ? (gameMap.GetCols() / 3) + 1 : ((gameMap.GetCols() / 3) * 2) + 2;
                        int iWidth = gameMap.GetCols() / 3;

                        delete people[i];
                        IslandConfig data = gameMap.GetIslandData(island);
                        bool isAggressive = (rand() % 2) == 0;

                        people[i] = new People(rand() % (iWidth - 2) + offset + 1,
                            rand() % (gameMap.GetRows() - 2) + 1,
                            island, data.pedestrianHealth, data.pedestrianAttack, isAggressive);
                    }
                }
            }

            // Ataque de cj al peaton
            if (GetAsyncKeyState(VK_SPACE)) {
                for (int i = 0; i < totalPeople; i++) {
                    if (people[i]->GetIsAlive()) {
                        Vec2 pPos = people[i]->GetPos();
                        if (abs(pPos.x - cj.GetX()) <= 1 && abs(pPos.y - cj.GetY()) <= 1) {

                            people[i]->TakeDamage(cj.GetAttackPower());

                            if (people[i]->GetHealth() <= 0) {
                                people[i]->Die(gameMap.GetMatrix());

                                // Condición de Victoria: Derrotar a Big Smoke
                                if (people[i]->GetIsBigSmoke()) {
                                    gameOverMessage = "¡ENHORABUENA! Has derrotado a Big Smoke y completado el juego.";
                                    currentState = GameState::GAME_OVER;
                                    break;
                                }

                                int island = people[i]->GetIsland();
                                int offset = (island == 0) ? 0 : (island == 1) ? (gameMap.GetCols() / 3) + 1 : ((gameMap.GetCols() / 3) * 2) + 2;
                                int iWidth = gameMap.GetCols() / 3;

                                delete people[i];
                                IslandConfig data = gameMap.GetIslandData(island);
                                bool isAggressive = (rand() % 2) == 0;

                                people[i] = new People(rand() % (iWidth - 2) + offset + 1,
                                    rand() % (gameMap.GetRows() - 2) + 1,
                                    island, data.pedestrianHealth, data.pedestrianAttack, isAggressive);
                            }
                        }
                    }
                }
            }

            // Gestión de peajes
            if (gameMap.GetMatrix()[cj.GetY()][cj.GetX()] == 'T') {
                bool isFirstToll = (cj.GetX() < gameMap.GetCols() / 2);
                int price = isFirstToll ? gameMap.GetTollOnePrice() : gameMap.GetTollTwoPrice();

                if (cj.GetMoney() >= price) {
                    cj.AddMoney(-price);
                    if (isFirstToll) {
                        gameMap.UnlockToll_one();
                        std::cout << "\n[PEAJE] ¡Has pagado " << price << "$ y has desbloqueado San Fierro!" << std::endl;
                    }
                    else {
                        gameMap.UnlockToll_two();
                        std::cout << "\n[PEAJE] ¡Has pagado " << price << "$ y has desbloqueado Las Venturas!" << std::endl;
                    }
                    Sleep(1500);
                }
                else {
                    // Derrota por control policial
                    gameOverMessage = "¡CONTROL POLICIAL! Detenido por no tener suficiente dinero (" + std::to_string(price) + "$).";
                    currentState = GameState::GAME_OVER;
                    break;
                }
            }

            // Gestión del dinero en el suelo
            if (!cj.IsInCar() && gameMap.GetMatrix()[cj.GetY()][cj.GetX()] == '$') {
                int islandIndex = 0;
                int islandWidth = (gameMap.GetCols() - 2) / 3;

                if (cj.GetX() > islandWidth && cj.GetX() <= islandWidth * 2 + 1) {
                    islandIndex = 1;
                }
                else if (cj.GetX() > islandWidth * 2 + 1) {
                    islandIndex = 2;
                }

                int maxMoney = gameMap.GetIslandData(islandIndex).maxMoneyPerPeo;
                int randomMoney = (rand() % maxMoney) + 1;
                cj.AddMoney(randomMoney);
                gameMap.GetMatrix()[cj.GetY()][cj.GetX()] = ' ';
            }

            gameMap.PrintView(cj.GetX(), cj.GetY(), cj.GetSymbol(), people, totalPeople, cj, cars, totalCars);
            Sleep(100);
            break;
        }

        case GameState::GAME_OVER: {
            // Estado Game Over: Muestra el mensaje de cierre durante 5 segundos y apaga el juego automáticamente
            std::cout << "====================================================" << std::endl;
            std::cout << "                     GAME OVER                      " << std::endl;
            std::cout << "====================================================" << std::endl;
            std::cout << "\n  " << gameOverMessage << std::endl;
            std::cout << "\n====================================================" << std::endl;
            std::cout << " Cerrando la aplicacion en 5 segundos..." << std::endl;
            Sleep(5000);
            goto cleanup; // Cierra el juego 
        }
        }
    }

cleanup:
    // Limpieza de memoria
    for (int i = 0; i < totalPeople; i++) {
        delete people[i];
    }
    delete[] people;

    for (int i = 0; i < totalCars; i++) delete cars[i];
    delete[] cars;

    return 0;
}
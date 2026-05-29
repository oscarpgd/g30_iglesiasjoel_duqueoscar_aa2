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
    int totalPeople = nLosSantos + nSanFierro;

    People** people = new People * [totalPeople];

    // Cochces
    const int carsInLosSantos = 3;
    const int carsInSanFierro = 2;
    const int carsInLasVenturas = 4;

    int totalCars = carsInLosSantos + carsInSanFierro + carsInLasVenturas;
    Car** cars = new Car * [totalCars];
    int carIndex = 0;

    int islandWidth = gameMap.GetCols() / 3;

    // Spawn de peatones - Los Santos (Isla 0)
    for (int i = 0; i < nLosSantos; i++) {
        int rx = rand() % (islandWidth - 2) + 1;
        int ry = rand() % (gameMap.GetRows() - 2) + 1;

        // 50% de probabilidad de ser agresivo
        bool isAggressive = (rand() % 2) == 0;
        IslandConfig data = gameMap.GetIslandData(0);

        people[i] = new People(rx, ry, 0, data.pedestrianHealth, data.pedestrianAttack, isAggressive);
    }

    // Spawn de peatones - San Fierro (Isla 1)
    for (int i = nLosSantos; i < totalPeople; i++) {
        int rx = rand() % (islandWidth - 2) + islandWidth + 1;
        int ry = rand() % (gameMap.GetRows() - 2) + 1;

        bool isAggressive = (rand() % 2) == 0;
        IslandConfig data = gameMap.GetIslandData(1);

        people[i] = new People(rx, ry, 1, data.pedestrianHealth, data.pedestrianAttack, isAggressive);
    }

    // Coches - Los Santos
    for (int i = 0; i < carsInLosSantos; i++) {
        int rx = rand() % (islandWidth - 2) + 1;
        int ry = rand() % (gameMap.GetRows() - 2) + 1;
        cars[carIndex++] = new Car(rx, ry, 0);
    }

    // Coches - San Fierro
    for (int i = 0; i < carsInSanFierro; i++) {
        int rx = rand() % (islandWidth - 2) + islandWidth + 1;
        int ry = rand() % (gameMap.GetRows() - 2) + 1;
        cars[carIndex++] = new Car(rx, ry, 1);
    }

    // Coches - Las Venturas
    for (int i = 0; i < carsInLasVenturas; i++) {
        int rx = rand() % (islandWidth - 2) + islandWidth * 2 + 2;
        int ry = rand() % (gameMap.GetRows() - 2) + 1;
        cars[carIndex++] = new Car(rx, ry, 2);
    }

    // Inicializacion daño y vida DEl player
    Player cj(5, 5, gameMap.GetCjMaxHealth(), gameMap.GetCjAttack());

    float deltaTime = 0.1f;

    // BUCLE PRINCIPAL
    while (true) {
        if (GetAsyncKeyState(VK_ESCAPE)) break;

        system("cls");

        // Update de CJ
        cj.Update(gameMap.GetMatrix(), gameMap.GetRows(), gameMap.GetCols(), people, totalPeople, cars, totalCars);
        Vec2 cjPos = { cj.GetX(), cj.GetY() };

        // Actualizar peatones
        for (int i = 0; i < totalPeople; i++) {
            people[i]->Update(gameMap.GetMatrix(), gameMap.GetRows(), gameMap.GetCols(), cjPos, deltaTime, cj);
        }

        // Atropello automático si vas en coche
        for (int i = 0; i < totalPeople; i++) {
            if (cj.IsInCar() && people[i]->GetIsAlive()) {
                Vec2 pPos = people[i]->GetPos();
                if (abs(pPos.x - cj.GetX()) <= 1 && abs(pPos.y - cj.GetY()) <= 1) {
                    people[i]->Die(gameMap.GetMatrix());

                    int island = people[i]->GetIsland();
                    int offset = (island == 0) ? 0 : (gameMap.GetCols() / 3) + 1;
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

                        // le aplica el daño al peaton
                        people[i]->TakeDamage(cj.GetAttackPower());

                        // Si la vida del peaton llega a 0 hace el procedimiento
                        if (people[i]->GetHealth() <= 0) {
                            people[i]->Die(gameMap.GetMatrix());

                            int island = people[i]->GetIsland();
                            int offset = (island == 0) ? 0 : (gameMap.GetCols() / 3) + 1;
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
            //Sleep(20);
        }

        // Abrir peajes
        // peaje 1
        int wall1 = gameMap.GetCols() / 3;
        int bridgeRow = gameMap.GetRows() / 2;
        if (!gameMap.IsTollOnePaid() && cj.GetY() == bridgeRow && cj.GetX() == wall1) {
            int price = gameMap.GetTollOnePrice();
            if (cj.GetMoney() >= price) {
                cj.AddMoney(-price);
                gameMap.UnlockToll_one();
            }
        }
        // peaje2
        int wall2 = (gameMap.GetCols() / 3) * 2 + 1;
        if (!gameMap.IsTollTwoPaid() && cj.GetY() == bridgeRow && cj.GetX() == wall2) {
            int price = gameMap.GetTollTwoPrice();
            if (cj.GetMoney() >= price) {
                cj.AddMoney(-price);
                gameMap.UnlockToll_two();
            }
        }

        // Gestión del dinero
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

        // vista horizontal
        gameMap.PrintView(cj.GetX(), cj.GetY(), cj.GetSymbol(), people, totalPeople, cj, cars, totalCars);
        Sleep(100);
    }

    // Limpieza de memoria
    for (int i = 0; i < totalPeople; i++) {
        delete people[i];
    }
    delete[] people;

    for (int i = 0; i < totalCars; i++) delete cars[i];
    delete[] cars;

    return 0;
}
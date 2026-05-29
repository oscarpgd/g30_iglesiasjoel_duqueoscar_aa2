#include <iostream>
#include <Windows.h>
#include <ctime>
#include "Map.h"
#include "Player.h"
#include "People.h"
#include "Car.h"

int main() {
    //INICIALIZACIÓN
    //random
    srand((time(NULL)));

    Map gameMap;

    //Lectura del archivo con un check
    if (!gameMap.LoadConfig("config.txt")) {
        std::cerr << "Error: No se pudo cargar config.txt" << std::endl;
        return 1;
    }

    //peatones
    int nLosSantos = gameMap.GetIslandData(0).numPeople;
    int nSanFierro = gameMap.GetIslandData(1).numPeople;
    int totalPeople = nLosSantos + nSanFierro;
    
    People** people = new People * [totalPeople];

    //Cochces
    const int carsInLosSantos = 3;
    const int carsInSanFierro = 2;
    const int carsInLasVenturas = 4;

    int totalCars = carsInLosSantos + carsInSanFierro + carsInLasVenturas;
    Car** cars = new Car * [totalCars];
    int carIndex = 0;

    // Cálculo del ancho de cada cuadrante (isla) 
    // Asumiendo que cols es el ancho total de las 3 islas
    int islandWidth = gameMap.GetCols() / 3;

    //Spawn de peatones - los santos
    for (int i = 0; i < nLosSantos; i++) 
    {
        int rx = rand() % (islandWidth - 2) + 1; // Dentro de los límites de la 1a isla
        int ry = rand() % (gameMap.GetRows() - 2) + 1;
        people[i] = new People(rx, ry, 0); //id 2
    }

    //Spanw de peatones - san fierro 
    for (int i = nLosSantos; i < totalPeople; i++) 
    {
        int rx = rand() % (islandWidth - 2) + islandWidth + 1; //salto de la primera pared
        int ry = rand() % (gameMap.GetRows() - 2) + 1;
        people[i] = new People(rx, ry, 1); //id 1
    }

    //COChes
    // Los Santos
    for (int i = 0; i < carsInLosSantos; i++) {
        int rx = rand() % (islandWidth - 2) + 1;
        int ry = rand() % (gameMap.GetRows() - 2) + 1;
        cars[carIndex++] = new Car(rx, ry, 0);
    }

    // San Fierro
    for (int i = 0; i < carsInSanFierro; i++) {
        int rx = rand() % (islandWidth - 2) + islandWidth + 1;
        int ry = rand() % (gameMap.GetRows() - 2) + 1;
        cars[carIndex++] = new Car(rx, ry, 1);
    }

    //las venturas
    for (int i = 0; i < carsInLasVenturas; i++) {
        int rx = rand() % (islandWidth - 2) + islandWidth * 2 + 2;
        int ry = rand() % (gameMap.GetRows() - 2) + 1;
        cars[carIndex++] = new Car(rx, ry, 2);
    }
    

    Player cj(5, 5); //posicion x e y
    //cj.AddMoney(10000);

    //BUCLE PRINCIPAL
    while (true) {
        if (GetAsyncKeyState(VK_ESCAPE)) break;
        
        system("cls");
        //Update de cj
        cj.Update(gameMap.GetMatrix(), gameMap.GetRows(), gameMap.GetCols(), people, totalPeople, cars, totalCars);
        Vec2 cjPos = { cj.GetX(), cj.GetY() };

        //Actualizar peatones
        for (int i = 0; i < totalPeople; i++) 
        {
            people[i]->Update(gameMap.GetMatrix(), gameMap.GetRows(), gameMap.GetCols(), cjPos);
        }

        //Atropellar peatones
        for (int i = 0; i < totalPeople; i++) 
        {
            // Atropello automático si vas en coche
            if (cj.IsInCar() && people[i]->GetIsAlive()) {
                Vec2 pPos = people[i]->GetPos();
                if (abs(pPos.x - cj.GetX()) <= 1 && abs(pPos.y - cj.GetY()) <= 1) {
                    people[i]->Die(gameMap.GetMatrix());

                    int island = people[i]->GetIsland();
                    int offset = (island == 0) ? 0 : (gameMap.GetCols() / 3) + 1;
                    int iWidth = gameMap.GetCols() / 3;

                    delete people[i];
                    people[i] = new People(rand() % (iWidth - 2) + offset + 1,
                        rand() % (gameMap.GetRows() - 2) + 1,
                        island);
                }
            }
        }

        //Recorre todos los peatones y comprueba su estado + Regen
        if (GetAsyncKeyState(VK_SPACE)) 
        {
            for (int i = 0; i < totalPeople; i++) 
            {
                if (people[i]->GetIsAlive()) 
                {
                    Vec2 pPos = people[i]->GetPos();
                    //Distancia de 1 casella
                    if (abs(pPos.x - cj.GetX()) <= 1 && abs(pPos.y - cj.GetY()) <= 1) {
                        people[i]->Die(gameMap.GetMatrix());

                        //Regeneración de un peaton nuevo una vez se uere uno
                        int island = people[i]->GetIsland();
                        int offset = (island == 0) ? 0 : (gameMap.GetCols() / 3) + 1;
                        int iWidth = gameMap.GetCols() / 3;

                        delete people[i];
                        people[i] = new People(rand() % (iWidth - 2) + offset + 1,
                            rand() % (gameMap.GetRows() - 2) + 1,
                            island);
                    }
                }
            }
        }
        //abrir peajes
        //Peaje 1
        int wall1 = gameMap.GetCols() / 3;
        int bridgeRow = gameMap.GetRows() / 2;
        if (!gameMap.IsTollOnePaid() && cj.GetY() == bridgeRow && cj.GetX() == wall1) 
        {
            int price = gameMap.GetTollOnePrice();
            if (cj.GetMoney() >= price) 
            {
                cj.AddMoney(-price); 
                gameMap.UnlockToll_one();
            }
            
        }
        // Peaje 2 - entre San Fierro y Las Venturas
        int wall2 = (gameMap.GetCols() / 3) * 2 + 1;
        if (!gameMap.IsTollTwoPaid() && cj.GetY() == bridgeRow && cj.GetX() == wall2) 
        {
            int price = gameMap.GetTollTwoPrice();
            if (cj.GetMoney() >= price) 
            {
                cj.AddMoney(-price);
                gameMap.UnlockToll_two();
            }
        }

        //Gestión del dinero
        if (!cj.IsInCar() && gameMap.GetMatrix()[cj.GetY()][cj.GetX()] == '$') 
        {
            //Identificar isla para maxMoney
            int islandIndex = 0;
            int islandWidth = (gameMap.GetCols() - 2) / 3;

            // San Fierro
            if (cj.GetX() > islandWidth && cj.GetX() <= islandWidth * 2 + 1) {
                islandIndex = 1; 
            }
            //Las venturas
            else if (cj.GetX() > islandWidth * 2 + 1) {
                islandIndex = 2;
            }

            //asignar diner MAX
            int maxMoney = gameMap.GetIslandData(islandIndex).maxMoneyPerPeo;
            //Generar cantidad aleatoria
            int randomMoney = (rand() % maxMoney) + 1;
            //sumar y limpiar la casilla
            cj.AddMoney(randomMoney);
            gameMap.GetMatrix()[cj.GetY()][cj.GetX()] = ' ';
        }

        //vista horizontal
        gameMap.PrintView(cj.GetX(), cj.GetY(), cj.GetSymbol(), people, totalPeople, cj, cars, totalCars);
        Sleep(100);
    }

    //Limpieza de memoria
    for (int i = 0; i < totalPeople; i++) {
        delete people[i];
    }
    delete[] people;

    for (int i = 0; i < totalCars; i++) delete cars[i];
    delete[] cars;

    return 0;
}
//
// Created by relin on 12/04/2021.
//

#include "GridManager.h"
#include <random>
#include <vector>
#include <fstream>
#include <GameManager.h>
#include "Workshop.h"

GridManager::GridManager() : minRange(10), parcourStep(3) {

}

void GridManager::init()
{
	grid.setActorAt(GameManager::getInstance()->addWorkshop("Market"), 0, 0);
}

int getRandomInt(int min = 0, int max = 1) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);

    return distrib(gen);
}

bool GridManager::canPlaceWorkshop(int x, int y) {

    for (int i = x - minRange / 2; i < x + minRange / 2; i++) {
        for (int j = y - minRange / 2; j < y + minRange / 2; j++) {
            if (grid.isOccupied(i, j)) {
                return false;
            }
        }
    }
    return true;
}

void GridManager::placeWorkshop(int x, int y, std::vector<std::shared_ptr<StaticActor>> &newWorkshops) {
    int d = getRandomInt(0, 4); // current direction; 0=RIGHT, 1=DOWN, 2=LEFT, 3=UP
    int s = 1; // chain size
    int c = 0;
    // starting point

    while (true) {
        for (int j = 0; j < 2; j++) {
            for (int i = 0; i < s; i += parcourStep) {
                if (canPlaceWorkshop(x, y)) {
                    grid.setActorAt(newWorkshops[c], x, y);
                    grid.updateBound(x, y);
                    c++;
                    if (c >= newWorkshops.size()) {
                        return;
                    }
                }
                switch (d) {
                    case 0:
                        y += parcourStep;
                        break;
                    case 1:
                        x += parcourStep;
                        break;
                    case 2:
                        y -= parcourStep;
                        break;
                    case 3:
                        x -= parcourStep;
                        break;
                    default:;
                }
            }
            d = (d + 1) % 4;
        }
        s = s + parcourStep;
    }
}

void GridManager::makeDebugFile() {
    std::ofstream file;
    file.open("../result.txt");

    for (int i = grid.getMinCoordinate().first - 1; i < grid.getMaxCoordinate().first; ++i) {
        for (int j = grid.getMinCoordinate().second - 1; j < grid.getMaxCoordinate().second; ++j) {
            if (grid.isOccupied(i, j)) {
                file << 1 << "\t";
            } else {
                file << 0 << "\t";
            }
        }
        file << std::endl;
    }
    file.close();
}

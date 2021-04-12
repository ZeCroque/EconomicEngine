//
// Created by relin on 12/04/2021.
//

#include "GridManager.h"
#include <random>
#include <vector>

GridManager::GridManager() : minRange(3), parcourStep(10) {

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
            if (grid.isOccupied(x, y)) {
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
                    grid.setActorAt(newWorkshops.at(c), x, y);
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

//
// Created by relin on 12/04/2021.
//

#include "GridManager.h"
#include <random>
#include <fstream>
#include <GameManager.h>
#include "Workshop.h"

GridManager::GridManager() : minRange(10), parcourStep(3) {

}


void GridManager::init() {
    grid.setActorAt(GameManager::getInstance()->addWorkshop("Market"), 0, 0);

    generationThread = std::thread([this]() { placeWorkshop(); });
}

int getRandomInt(int min = 0, int max = 1) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);

    return distrib(gen);
}

bool GridManager::canPlaceWorkshop(int x, int y) {

    int range = minRange + getRandomInt(-1, 1);

    for (int i = x - range / 2; i < x + range / 2; i++) {
        for (int j = y - range / 2; j < y + range / 2; j++) {
            if (grid.isOccupied(i, j)) {
                return false;
            }
        }
    }
    return true;
}

void GridManager::placeWorkshop() {
    int d = getRandomInt(0, 4); // current direction; 0=RIGHT, 1=DOWN, 2=LEFT, 3=UP
    int s = 1; // chain size
    int x = 0;
    int y = 0;

    while (true) {
        for (int j = 0; j < 2; j++) {
            for (int i = 0; i < s; i += parcourStep) {
                while (workshopQueue.empty() && GameManager::getInstance()->getIsRunning());
                if (!GameManager::getInstance()->getIsRunning()) {
                    return;
                }
                if (canPlaceWorkshop(x, y)) {
                    grid.setActorAt(workshopQueue.front(), x, y);
                    workshopQueue.pop();
                    grid.updateBounds(x, y);
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
                file << grid.getActorAt(i, j)->getId() << "\t";
            } else {
                file << 0 << "\t";
            }
        }
        file << std::endl;
    }
    file.close();
}

std::thread &GridManager::getGenerationThread() {
    return generationThread;
}

void GridManager::queueWorkshop(std::shared_ptr<Workshop> workshop) { // NOLINT(performance-unnecessary-value-param)
    workshopQueue.emplace(workshop);
}

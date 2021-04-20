//
// Created by relin on 12/04/2021.
//

#include "GridManager.h"
#include <random>
#include <fstream>
#include <GameManager.h>
#include "Workshop.h"
#include <iostream>

GridManager::GridManager() : minRange(10), parcourStep(3), maxDistanceToMarket(25) {

}


void GridManager::init() {
    grid.setActorAt(GameManager::getInstance()->addWorkshop("Market"), 0, 0);
    marketCoordinate.emplace_back(std::pair<int, int>(0, 0));

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
                while (workshopQueue.empty() && GameManager::getInstance()->getIsRunning()) std::cout << "done" << std::endl;
                if (!GameManager::getInstance()->getIsRunning() && GameManager::getInstance()->getHasEverRun()) {
                    makeDebugFile();
                    return;
                }
                if (!workshopQueue.empty() && canPlaceWorkshop(x, y)) {
                    if (haveMarketInRange(x, y)) {
                        grid.setActorAt(workshopQueue.front(), x, y);
                        workshopQueue.pop();
                    } else {
                        grid.setActorAt(GameManager::getInstance()->addWorkshop("Market"), x, y);
                        marketCoordinate.emplace_back(std::pair<int, int>(x, y));
                    }
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

	for (int y = grid.getMinCoordinate().second - 1; y < grid.getMaxCoordinate().second; ++y)
	{
		for (int x = grid.getMinCoordinate().first - 1; x < grid.getMaxCoordinate().first; ++x)
		{
            if (grid.isOccupied(x, y)) {
                file << grid.getActorAt(x, y)->getId() << "\t";
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

int GridManager::getClosestMarketCoordinate(int x, int y) {
    int closest = maxDistanceToMarket + 1;
    int current;

    for (auto coord : marketCoordinate) {
        current = int(sqrt(pow(coord.first - x, 2) + pow(coord.second - y, 2) * 1.0));
        if (current <= closest) {
            closest = current;
        }
    }
    return closest;
}

bool GridManager::haveMarketInRange(int x, int y) {

    if (std::any_of(marketCoordinate.begin(), marketCoordinate.end(), [x, y, this](std::pair<int, int> coord) {
        return int(sqrt(pow(coord.first - x, 2) + pow(coord.second - y, 2) * 1.0)) < maxDistanceToMarket;
    })) {
        return true;
    } else {
        return false;
    }
}


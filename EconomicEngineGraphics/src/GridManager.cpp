//
// Created by relin on 12/04/2021.
//

#include "GridManager.h"
#include <random>
#include <fstream>
#include <GameManager.h>
#include "Workshop.h"

GridManager::GridManager() : minRange(6), pathStep(4), maxDistanceToMarket(25), isGenerationThreadRunning(false) {}


void GridManager::init() {
    grid.setActorAt(GameManager::getInstance()->addWorkshop("Market"), 0, 0);
    marketsCoordinates.emplace_back(std::pair<int, int>(0, 0));

	isGenerationThreadRunning = true;
	generationThread = std::thread([this]() { placeWorkshop(); });
}

void GridManager::reset()
{
    isGenerationThreadRunning = false;
	generationThread.join();
	
	while(!workshopQueue.empty())
	{
		workshopQueue.pop();
	}

	marketsCoordinates.clear();
	grid.reset();
}

int getRandomInt(const int min = 0, const int max = 1) {
    std::random_device rd;
    std::mt19937 gen(rd());
    const std::uniform_int_distribution<> distribution(min, max);

    return static_cast<int>(distribution(gen));
}

bool GridManager::canPlaceWorkshop(int x, int y) const
{
	const int range = minRange;
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
            for (int i = 0; i < s; i += pathStep) {
                while (workshopQueue.empty() && GameManager::getInstance()->getIsRunning() && isGenerationThreadRunning ||
                       !GameManager::getInstance()->getIsInitialized()) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(GameManager::maxFPS));
                }
                if (!GameManager::getInstance()->getIsRunning() && GameManager::getInstance()->getIsInitialized() || !isGenerationThreadRunning) {
                    makeDebugFile();
                    return;
                }

                int noiseX = getRandomInt(x - minRange / 5, x + minRange / 5);
                int noiseY = getRandomInt(y - minRange / 5, y + minRange / 5);
                if (!workshopQueue.empty() && canPlaceWorkshop(noiseX, noiseY)) {
                	grid.updateBounds(noiseX, noiseY);
                    if (haveMarketInRange(noiseX, noiseY)) {
                        auto &ws = workshopQueue.front();
                        workshopQueue.pop();
                    	grid.setActorAt(ws, noiseX, noiseY);
                        ws->setClosestMarketCoordinate(getClosestMarket(noiseX, noiseY));
                    } else {
                    	
                        grid.setActorAt(GameManager::getInstance()->addWorkshop("Market"), noiseX, noiseY);
                        marketsCoordinates.emplace_back(std::pair<int, int>(noiseX, noiseY));
                        updateClosestMarket(noiseX, noiseX);
                    }
                    GameManager::getInstance()->setBackgroundNeedsUpdate(true);
                }

                switch (d) {
                    case 0:
                        y += pathStep;
                        break;
                    case 1:
                        x += pathStep;
                        break;
                    case 2:
                        y -= pathStep;
                        break;
                    case 3:
                        x -= pathStep;
                        break;
                    default:;
                }
            }
            d = (d + 1) % 4;
        }
        s = s + pathStep;
    }
}

void GridManager::makeDebugFile() const
{
    std::ofstream file;
    file.open("../result.txt");

    for (int y = grid.getMinCoordinate().second - 1; y < grid.getMaxCoordinate().second; ++y) {
        for (int x = grid.getMinCoordinate().first - 1; x < grid.getMaxCoordinate().first; ++x) {
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

Grid& GridManager::getGrid() const
{
	return grid;
}

void GridManager::queueWorkshop(std::shared_ptr<Workshop> inWorkshop) { // NOLINT(performance-unnecessary-value-param)
    workshopQueue.emplace(inWorkshop);
}

bool GridManager::getIsGenerationThreadRunning() const
{
	return isGenerationThreadRunning;
}

int GridManager::getClosestMarketCoordinate(int x, int y) {
    int closest = maxDistanceToMarket + 1;

    for (const auto& marketCoordinates : marketsCoordinates) {
        const auto current = static_cast<int>(sqrt(pow(marketCoordinates.first - x, 2) + pow(marketCoordinates.second - y, 2)));
        if (current <= closest) {
            closest = current;
        }
    }
    return closest;
}

bool GridManager::haveMarketInRange(int x, int y) {

    if (std::ranges::any_of(marketsCoordinates.begin(), marketsCoordinates.end(), [x, y, this](const std::pair<int, int> coordinates) {
        return static_cast<int>(sqrt(pow(coordinates.first - x, 2) + pow(coordinates.second - y, 2))) < maxDistanceToMarket;
    })) {
        return true;
    } else {
        return false;
    }
}

std::pair<int, int> GridManager::getClosestMarket(const int x, const int y) {
    double bestDistance = INT8_MAX;
    std::pair<int, int> result;

    for (const auto& market : marketsCoordinates) {
	    const double distance = sqrt(pow(market.first - x, 2) + pow(market.second - y, 2));
        if (distance < bestDistance) {
            bestDistance = distance;
            result = market;
        }
    }

    return result;
}

void GridManager::updateClosestMarket(int x, int y) {

    auto const yMin = std::max(grid.getMinCoordinate().second, y - maxDistanceToMarket / 2); //Todo: optimize chunk
    auto const yMax = std::min(grid.getMaxCoordinate().second, y + maxDistanceToMarket / 2);
    auto const xMin = std::max(grid.getMinCoordinate().first, x - maxDistanceToMarket / 2);
    auto const xMax = std::min(grid.getMaxCoordinate().first, x + maxDistanceToMarket / 2);

    for (int a = yMin; a <= yMax; ++a) {
        for (int b = xMin; b <= xMax; ++b) {
            auto* cell = grid.getActorAt(b, a);
            if (cell) {
                cell->setClosestMarketCoordinate(getClosestMarket(x, y));
            }
        }
    }
}


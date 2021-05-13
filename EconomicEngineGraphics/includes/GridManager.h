//
// Created by relin on 12/04/2021.
//

#ifndef ECONOMICENGINE_GRIDMANAGER_H
#define ECONOMICENGINE_GRIDMANAGER_H


#include <vector>
#include "Grid.h"
#include <queue>


class GridManager {
public:

    GridManager();

    void init();

	void reset();

    bool canPlaceWorkshop(int inX, int inY) const;

    void queueWorkshop(std::shared_ptr<Workshop> inWorkshop);

    [[nodiscard]] bool getIsGenerationThreadRunning() const;
	
    [[nodiscard]] std::thread& getGenerationThread();

	[[nodiscard]] Grid& getGrid() const;
	
#ifndef NDEBUG
    void makeDebugFile() const;
#endif

	static int getRandomInt(const int min = 0, const int max = 1);

private :

    mutable Grid grid;
    int minRange;
    int pathStep;
    int maxDistanceToMarket;

    std::queue<std::shared_ptr<Workshop>> workshopQueue;

	bool isGenerationThreadRunning;
    std::thread generationThread;

    void placeWorkshop();

    bool isAnyMarketInRange(int inX, int inY);

    std::pair<int, int> getClosestMarket(int inX, int inY);

    void updateClosestMarket(int inX, int inY);

    int getClosestMarketCoordinate(int inX, int inY);

    std::vector<std::pair<int, int>> marketsCoordinates;

};


#endif //ECONOMICENGINE_GRIDMANAGER_H

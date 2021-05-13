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

    explicit GridManager();

    void init();

	void reset();

    bool canPlaceWorkshop(int x, int y) const;

    void queueWorkshop(std::shared_ptr<Workshop> workshop);

    [[nodiscard]] bool getIsGenerationThreadRunning() const;
	
    [[nodiscard]] std::thread &getGenerationThread();

	[[nodiscard]] Grid& getGrid() const;

    void makeDebugFile() const;

private :

    mutable Grid grid;
    int minRange;
    int pathStep;
    int maxDistanceToMarket;

    std::queue<std::shared_ptr<Workshop>> workshopQueue;

	bool isGenerationThreadRunning;
    std::thread generationThread;

    void placeWorkshop();

    bool haveMarketInRange(int x, int y);

    std::pair<int, int> getClosestMarket(int x, int y);

    void updateClosestMarket(int x, int y);

    int getClosestMarketCoordinate(int x, int y);

    std::vector<std::pair<int, int>> marketsCoordinates;

};


#endif //ECONOMICENGINE_GRIDMANAGER_H

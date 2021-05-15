#ifndef GRID_MANAGER_H
#define GRID_MANAGER_H

#include <vector>
#include <queue>

#include "Grid.h"

class GridManager
{
public:

    GridManager();

    void init();

	void reset();

    bool canPlaceWorkshop(int inX, int inY) const;

    void queueWorkshop(std::shared_ptr<Workshop> inWorkshop);

    [[nodiscard]] bool isGenerationThreadRunning() const;
	
    [[nodiscard]] std::thread& getGenerationThread();

	[[nodiscard]] Grid& getGrid() const;
	
#ifndef NDEBUG
    void makeDebugFile() const;
#endif

	static int getRandomInt(int min = 0, int max = 1);

private :

    mutable Grid grid;
    int minRange;
    int pathStep;
    int maxDistanceToMarket;

    std::queue<std::shared_ptr<Workshop>> workshopQueue;

	bool bIsGenerationThreadRunning;
    std::thread generationThread;

    void placeWorkshop();

    bool isAnyMarketInRange(int inX, int inY);

    std::pair<int, int> getClosestMarket(int inX, int inY);

    void updateClosestMarket(int inX, int inY);

    int getClosestMarketCoordinate(int inX, int inY);

    std::vector<std::pair<int, int>> marketsCoordinates;

};


#endif //GRID_MANAGER_H

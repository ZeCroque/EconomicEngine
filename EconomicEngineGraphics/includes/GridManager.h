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

private :
    void placeWorkshops();

	void updateClosestMarket(int inX, int inY);

	#ifndef NDEBUG
    void makeDebugFile() const;
#endif

    bool isAnyMarketInRange(int inX, int inY);

    std::pair<int, int> getClosestMarket(int inX, int inY);

    int getClosestMarketCoordinate(int inX, int inY);

	static int getRandomInt(int min = 0, int max = 1);
	
	mutable Grid grid;

	std::queue<std::shared_ptr<Workshop>> workshopQueue;
    int pathStep;
	int minRangeBetweenWorkshops;

	std::vector<std::pair<int, int>> marketsCoordinates;
    int maxDistanceToMarket;

	bool bIsGenerationThreadRunning;
    std::thread generationThread;

};


#endif //GRID_MANAGER_H

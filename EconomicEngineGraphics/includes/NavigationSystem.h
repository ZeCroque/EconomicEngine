#ifndef NAVIGATION_SYSTEM_H
#define NAVIGATION_SYSTEM_H

#include <list>
#include <mutex>

class Grid;
struct Node;

class NavigationSystem {

public:
	static std::list<std::pair<int, int>> aStarResolution(Grid& grid, const std::pair<int, int>& startingCoordinates,
	                                                      const std::pair<int, int>& objectiveCoordinates);
	static void drawPath(Grid& grid, const std::pair<int, int>& startingCoordinates, const std::pair<int, int>& objectiveCoordinates);

	//TODO fix this lint
	inline static std::mutex mutex;

private:
	static float getHeuristicDistance(const Node* firstNode, const Node* secondNode);
};
#endif // NAVIGATION_SYSTEM_H
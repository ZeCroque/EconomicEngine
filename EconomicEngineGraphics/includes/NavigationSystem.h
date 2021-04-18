#ifndef NAVIGATION_SYSTEM_H
#define NAVIGATION_SYSTEM_H

#include <stack>
#include <vector>
class Grid;
struct Node;

class NavigationSystem {

public:
	static std::stack<std::pair<int, int>> aStarResolution(Grid& grid, const std::pair<int, int>& startingCoordinates, const std::pair<int, int>& objectiveCoordinates);
	static void drawPath(Grid& grid, const std::pair<int, int>& startingCoordinates, const std::pair<int, int>& objectiveCoordinates);

private:
	static float getHeuristicDistance(const Node* firstNode, const Node* secondNode);
};
#endif // NAVIGATION_SYSTEM_H
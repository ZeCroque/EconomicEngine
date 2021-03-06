#ifndef NAVIGATION_SYSTEM_H
#define NAVIGATION_SYSTEM_H

#include <list>
#include <set>

class Grid;
struct Node;

class NavigationSystem {

	using Rectangle = const std::pair<std::pair<int, int>, std::pair<int, int>>;
public:
	static std::list<std::pair<int, int>> aStarResolution(Grid& inGrid, const std::pair<int, int>& inStartingCoordinates,
	                                                      const std::pair<int, int>& inObjectiveCoordinates);
	
	static void updateNeighborParent(std::list<Node*>& outNodesToTest, std::set<Node*>&outModifiedNodes, Node* inCurrentNode, Node* inObjectiveNode, Node* inNodeNeighbor);

private:
#ifndef NDEBUG
	static void drawPath(Grid& inGrid, const std::list<std::pair<int, int>>& inPath, const Rectangle& inBounds, const std::pair<int, int>& inStartingCoords, const std::pair<int, int>& inObjectiveCoords);
#endif
	
	static float getHeuristicDistance(const Node* inFirstNode, const Node* inSecondNode);
};
#endif // NAVIGATION_SYSTEM_H
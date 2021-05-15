#include "NavigationSystem.h"

#ifndef NDEBUG
#include <iostream>
#endif

#include "Grid.h"

std::list<std::pair<int, int>> NavigationSystem::aStarResolution(
	Grid& inGrid, const std::pair<int, int>& inStartingCoordinates, const std::pair<int, int>& inObjectiveCoordinates)
{
	auto* objectiveNode = &inGrid.getNodeAt(inObjectiveCoordinates.first, inObjectiveCoordinates.second);
	auto* startingNode = &inGrid.getNodeAt(inStartingCoordinates.first, inStartingCoordinates.second);
	startingNode->localGoal = 0.0f;
	startingNode->globalGoal = getHeuristicDistance(startingNode, objectiveNode);
	std::list<Node*> nodesToTest;
	std::set<Node*> modifiedNodes;
	Node* currentNode = startingNode;
	nodesToTest.emplace_back(currentNode);
	const Rectangle searchBounds = std::pair(
		std::pair(std::min(inStartingCoordinates.first, inObjectiveCoordinates.first) - 1, std::min(inStartingCoordinates.second, inObjectiveCoordinates.second) - 2),
		std::pair(std::max(inStartingCoordinates.first, inObjectiveCoordinates.first) + 1, std::max(inStartingCoordinates.second, inObjectiveCoordinates.second) + 1)
	);
	while (!nodesToTest.empty())
	{
		// Sorts nodes by globalGoal,the smaller being the closest to objective
		nodesToTest.sort([](const Node* lhs, const Node* rhs)
		{
			return lhs->globalGoal < rhs->globalGoal;
		});
		// Yet visited nodes removal
		while (!nodesToTest.empty() && nodesToTest.front()->visited)
		{
			nodesToTest.pop_front();
		}
		// If we tested all the required nodes, then we end the pathfinding
		if (nodesToTest.empty())
		{
			break;
		}
		currentNode = nodesToTest.front();
		currentNode->visited = true;
		modifiedNodes.emplace(currentNode);
		// Upper current neighbor
		if (currentNode->y - 1 >= searchBounds.first.second)
		{
			updateNeighborParent(nodesToTest, modifiedNodes, currentNode, objectiveNode, &inGrid.getNodeAt(currentNode->x, currentNode->y - 1));
		}
		// Lower current neighbor
		if (currentNode->y + 1 <= searchBounds.second.second)
		{
			updateNeighborParent(nodesToTest, modifiedNodes, currentNode, objectiveNode, &inGrid.getNodeAt(currentNode->x, currentNode->y + 1));
		}
		// Left current neighbor
		if (currentNode->x - 1 >= searchBounds.first.first)
		{
			updateNeighborParent(nodesToTest, modifiedNodes, currentNode, objectiveNode, &inGrid.getNodeAt((currentNode->x - 1), currentNode->y));
		}
		// Right current neighbor
		if (currentNode->x + 1 <= searchBounds.second.first)
		{
			updateNeighborParent(nodesToTest, modifiedNodes, currentNode, objectiveNode, &inGrid.getNodeAt((currentNode->x + 1), currentNode->y));
		}
	}
	std::list<std::pair<int,int>> returnPath;
	if (objectiveNode->parent) 
	{
		auto* parent = objectiveNode->parent;
		while (parent != startingNode)
		{
			returnPath.emplace_front(std::pair(parent->x, parent->y));
			parent = parent->parent;
		}
	}
	returnPath.emplace_front(inStartingCoordinates);
	returnPath.emplace_back(inObjectiveCoordinates);
#ifndef NDEBUG
	drawPath(inGrid, returnPath, searchBounds, inStartingCoordinates, inObjectiveCoordinates);
#endif
	for (auto* modifiedNode : modifiedNodes)
	{
		modifiedNode->resetNode();
	}
	return returnPath;
}

void NavigationSystem::updateNeighborParent(std::list<Node*>& outNodesToTest, std::set<Node*>& outModifiedNodes, Node* inCurrentNode, Node* inObjectiveNode, Node* inNodeNeighbor)
{
	// If the neighbor is navigable and not yet visited then we add add it to the nodes to test list
	if (!inNodeNeighbor->visited && !inNodeNeighbor->isOccupied())
	{
		outNodesToTest.emplace_back(inNodeNeighbor);
	}
	// If the potential cost is smaller than the actual cost (i.e. the quickest way to go to the neighbor node is to go through the currentNode) 
	if (const float possiblyLowerGoal = inCurrentNode->localGoal + getHeuristicDistance(inCurrentNode, inNodeNeighbor); possiblyLowerGoal < inNodeNeighbor->localGoal)
	{
		// We update the neighbor node
		inNodeNeighbor->parent = inCurrentNode;
		inNodeNeighbor->localGoal = possiblyLowerGoal;
		inNodeNeighbor->globalGoal = inNodeNeighbor->localGoal + getHeuristicDistance(inNodeNeighbor, inObjectiveNode);
		outModifiedNodes.emplace(inNodeNeighbor);
	}
}

float NavigationSystem::getHeuristicDistance(const Node* inFirstNode, const Node* inSecondNode)
{
	return sqrtf(static_cast<float>((inFirstNode->x - inSecondNode->x) * (inFirstNode->x - inSecondNode->x) + (inFirstNode->y - inSecondNode->y) * (inFirstNode->y - inSecondNode->y)));
}


#ifndef NDEBUG
void NavigationSystem::drawPath(Grid& inGrid, const std::list<std::pair<int, int>>& inPath, const Rectangle& inBounds, const std::pair<int, int>& inStartingCoords, const std::pair<int, int>& inObjectiveCoords)
{
	std::cout << "Starting : [" << inStartingCoords.first << ";" << inStartingCoords.second << "]" << std::endl;
	std::cout << "Objective : [" << inObjectiveCoords.first << ";" << inObjectiveCoords.second << "]" << std::endl;
	std::cout << "MinBound : [" << inBounds.first.first << ";" << inBounds.first.second << "]" << std::endl;
	std::cout << "MaxBound : [" <<  inBounds.second.first << ";" << inBounds.second.second << "]" << std::endl;
	std::cout << "PathSize : " << inPath.size() << std::endl;


	for (int y = inBounds.first.second; y <= inBounds.second.second; ++y)
	{
		for (int x = inBounds.first.first; x <= inBounds.second.first; ++x)
		{
			char isInPath = '0';
			for (char i = 0; const auto& coordinate : inPath)
			{
				if (coordinate.first == x && coordinate.second == y)
				{
					if (i>9)
					{
						isInPath = static_cast<char>(i + 'A' - 10);
						break;
					}
					isInPath = static_cast<char>(i + '0');
					break;
				}
				++i;
			}
			if(inGrid.isOccupied(x,y))
			{
				isInPath = '#';
			}
			if (inStartingCoords == std::pair(x,y))
			{
				isInPath = '+';
			}
			
			std::cout << isInPath;
		}
		std::cout << std::endl;
	}
}
#endif
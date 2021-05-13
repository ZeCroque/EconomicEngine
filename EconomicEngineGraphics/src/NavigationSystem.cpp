#include "NavigationSystem.h"

#include <iostream>

#include "Grid.h"

std::list<std::pair<int, int>> NavigationSystem::aStarResolution(
	Grid& grid, const std::pair<int, int>& startingCoordinates, const std::pair<int, int>& objectiveCoordinates)
{
	auto* objectiveNode = &grid.getNodeAt(objectiveCoordinates.first, objectiveCoordinates.second);
	auto* startingNode = &grid.getNodeAt(startingCoordinates.first, startingCoordinates.second);
	startingNode->localGoal = 0.0f;
	startingNode->globalGoal = getHeuristicDistance(startingNode, objectiveNode);
	std::list<Node*> nodesToTest;
	std::set<Node*> modifiedNodes;
	Node* currentNode = startingNode;
	nodesToTest.emplace_back(currentNode);
	const std::pair<std::pair<int, int>, std::pair<int,int>> searchBounds = std::pair(
		std::pair(std::min(startingCoordinates.first, objectiveCoordinates.first) - 1, std::min(startingCoordinates.second, objectiveCoordinates.second) - 2),
		std::pair(std::max(startingCoordinates.first, objectiveCoordinates.first) + 1, std::max(startingCoordinates.second, objectiveCoordinates.second) + 1)
	);
	while (!nodesToTest.empty())
	{
		// Sort des nodes par ordre de globalGoal, celui ayant le plus petit étant le plus proche de l'objectif
		nodesToTest.sort([](const Node* lhs, const Node* rhs)
		{
			return lhs->globalGoal < rhs->globalGoal;
		});
		// On enlève les nodes déjà visités
		while (!nodesToTest.empty() && nodesToTest.front()->visited)
		{
			nodesToTest.pop_front();
		}
		// Si on a vidé la liste lors de la dernière opération, on break
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
			updateNeighborParent(nodesToTest, modifiedNodes, currentNode, objectiveNode, &grid.getNodeAt(currentNode->x, currentNode->y - 1));
		}
		// Lower current neighbor
		if (currentNode->y + 1 <= searchBounds.second.second)
		{
			updateNeighborParent(nodesToTest, modifiedNodes, currentNode, objectiveNode, &grid.getNodeAt(currentNode->x, currentNode->y + 1));
		}
		// Left current neighbor
		if (currentNode->x - 1 >= searchBounds.first.first)
		{
			updateNeighborParent(nodesToTest, modifiedNodes, currentNode, objectiveNode, &grid.getNodeAt((currentNode->x - 1), currentNode->y));
		}
		// Right current neighbor
		if (currentNode->x + 1 <= searchBounds.second.first)
		{
			updateNeighborParent(nodesToTest, modifiedNodes, currentNode, objectiveNode, &grid.getNodeAt((currentNode->x + 1), currentNode->y));
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
	returnPath.emplace_front(startingCoordinates);
	returnPath.emplace_back(objectiveCoordinates);
	// drawPath(grid, returnPath, searchBounds, startingCoordinates, objectiveCoordinates); //TODO remove debug
	for (auto* modifiedNode : modifiedNodes)
	{
		modifiedNode->resetNode();
	}
	return returnPath;
}

void NavigationSystem::drawPath(Grid& inGrid, const std::list<std::pair<int, int>>& inPath, const std::pair<std::pair<int, int>, std::pair<int, int>>&
                                inBounds, const std::pair<int, int>& inStartingCoords, const std::pair<int, int>& inObjectiveCoords)
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
						isInPath = i + 'A' - 10;
						break;
					}
					isInPath = i + '0';
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

void NavigationSystem::updateNeighborParent(std::list<Node*>& outNodesToTest, std::set<Node*>& outModifiedNodes, Node* inCurrentNode, Node* inObjectiveNode, Node* inNodeNeighbor)
{
	// Si le voisin n'est pas déjà visité et qu'il est navigable on l'ajoute aux nodes à tester
	if (!inNodeNeighbor->visited && !inNodeNeighbor->isOccupied())
	{
		outNodesToTest.emplace_back(inNodeNeighbor);
	}

	// Si ce "coût" potentiel est inférieur à son "coût" actuel et donc que venir par ce node est le chemin le plus rapide pour arriver à ce node voisin
	if (const float possiblyLowerGoal = inCurrentNode->localGoal + getHeuristicDistance(inCurrentNode, inNodeNeighbor); possiblyLowerGoal < inNodeNeighbor->localGoal)
	{
		// On met à jour le node voisin
		inNodeNeighbor->parent = inCurrentNode;
		inNodeNeighbor->localGoal = possiblyLowerGoal;
		inNodeNeighbor->globalGoal = inNodeNeighbor->localGoal + getHeuristicDistance(inNodeNeighbor, inObjectiveNode);
	}
	outModifiedNodes.emplace(inNodeNeighbor);
}


float NavigationSystem::getHeuristicDistance(const Node* firstNode, const Node* secondNode)
{
	return sqrtf(static_cast<float>((firstNode->x - secondNode->x) * (firstNode->x - secondNode->x) + (firstNode->y - secondNode->y) * (firstNode->y - secondNode->y)));
}

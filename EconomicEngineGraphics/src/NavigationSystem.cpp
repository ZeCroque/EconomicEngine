#include "NavigationSystem.h"

#include <iostream>
#include <list>

#include "Grid.h"

std::list<std::pair<int, int>> NavigationSystem::aStarResolution(
	Grid& inGrid, const std::pair<int, int>& inStartingCoordinates, const std::pair<int, int>& inObjectiveCoordinates)
{
	auto* objectiveNode = &inGrid.getNodeAt(inObjectiveCoordinates.first, inObjectiveCoordinates.second);
	auto* startingNode = &inGrid.getNodeAt(inStartingCoordinates.first, inStartingCoordinates.second);
	startingNode->localGoal = 0.0f;
	startingNode->globalGoal = getHeuristicDistance(startingNode, objectiveNode);
	std::list<Node*> nodesToTest;
	std::list<Node*> modifiedNodes;
	Node* currentNode = startingNode;
	nodesToTest.emplace_back(currentNode);
	const std::pair<std::pair<int, int>, std::pair<int,int>> searchBounds = std::pair(
		std::pair(std::min(inStartingCoordinates.first, inObjectiveCoordinates.first) - 1, std::min(inStartingCoordinates.second, inObjectiveCoordinates.second) - 2),
		std::pair(std::max(inStartingCoordinates.first, inObjectiveCoordinates.first) + 1, std::max(inStartingCoordinates.second, inObjectiveCoordinates.second) + 1)
	);
	while (!nodesToTest.empty())
	{
		// Sort des nodes par ordre de globalGoal, celui ayant le plus petit �tant le plus proche de l'objectif
		nodesToTest.sort([](const Node* lhs, const Node* rhs)
		{
			return lhs->globalGoal < rhs->globalGoal;
		});
		// On enl�ve les nodes d�j� visit�s
		while (!nodesToTest.empty() && nodesToTest.front()->visited)
		{
			nodesToTest.pop_front();
		}
		// Si on a vid� la liste lors de la derni�re op�ration, on break
		if (nodesToTest.empty())
		{
			break;
		}
		currentNode = nodesToTest.front();
		currentNode->visited = true;
		modifiedNodes.emplace_back(currentNode);
		// Upper current neighbor
		if (currentNode->y - 1 >= searchBounds.first.second)
		{
			updateNeighborParent(nodesToTest, currentNode, objectiveNode, &inGrid.getNodeAt(currentNode->x, currentNode->y - 1));
		}
		// Lower current neighbor
		if (currentNode->y + 1 <= searchBounds.second.second)
		{
			updateNeighborParent(nodesToTest, currentNode, objectiveNode, &inGrid.getNodeAt(currentNode->x, currentNode->y + 1));
		}
		// Left current neighbor
		if (currentNode->x - 1 >= searchBounds.first.first)
		{
			updateNeighborParent(nodesToTest, currentNode, objectiveNode, &inGrid.getNodeAt((currentNode->x - 1), currentNode->y));
		}
		// Right current neighbor
		if (currentNode->x + 1 <= searchBounds.second.first)
		{
			updateNeighborParent(nodesToTest, currentNode, objectiveNode, &inGrid.getNodeAt((currentNode->x + 1), currentNode->y));
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
			int i = 0;
			for (const auto& coordinate : inPath)
			{
				if (coordinate.first == x && coordinate.second == y)
				{
					if (i>9)
					{
						isInPath = i+'A'-10;
						break;
					}
					isInPath = i+'0';
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

void NavigationSystem::updateNeighborParent(std::list<Node*>& outNodesToTest, Node* inCurrentNode, Node* inObjectiveNode, Node* inNodeNeighbor)
{
	// Si le voisin n'est pas d�j� visit� et qu'il est navigable on l'ajoute aux nodes � tester
	if (!inNodeNeighbor->visited && !inNodeNeighbor->isOccupied())
	{
		outNodesToTest.emplace_back(inNodeNeighbor);
	}

	// Calcul du "co�t" total potentiel pour naviguer � ce node voisin
	const float possiblyLowerGoal = inCurrentNode->localGoal + getHeuristicDistance(inCurrentNode, inNodeNeighbor);
	// Si ce "co�t" potentiel est inf�rieur � son "co�t" actuel et donc que venir par ce node est le chemin le plus rapide pour arriver � ce node voisin
	if (possiblyLowerGoal < inNodeNeighbor->localGoal)
	{
		// On met � jour le node voisin
		inNodeNeighbor->parent = inCurrentNode;
		inNodeNeighbor->localGoal = possiblyLowerGoal;
		inNodeNeighbor->globalGoal = inNodeNeighbor->localGoal + getHeuristicDistance(inNodeNeighbor, inObjectiveNode);
	}
}


float NavigationSystem::getHeuristicDistance(const Node* inFirstNode, const Node* inSecondNode)
{
	return sqrtf(static_cast<float>((inFirstNode->x - inSecondNode->x) * (inFirstNode->x - inSecondNode->x) + (inFirstNode->y - inSecondNode->y) * (inFirstNode->y - inSecondNode->y)));
}

#include "NavigationSystem.h"

#include <iostream>
#include <list>

#include "Grid.h"

std::list<std::pair<int, int>> NavigationSystem::aStarResolution(
	Grid& grid, const std::pair<int, int>& startingCoordinates, const std::pair<int, int>& objectiveCoordinates)
{
	mutex.lock();
	auto* objectiveNode = &grid.getNodeAt(objectiveCoordinates.first, objectiveCoordinates.second);
	auto* startingNode = &grid.getNodeAt(startingCoordinates.first, startingCoordinates.second);
	startingNode->localGoal = 0.0f;
	startingNode->globalGoal = getHeuristicDistance(startingNode, objectiveNode);
	std::list<Node*> nodesToTest;
	std::list<Node*> modifiedNodes;
	Node* currentNode = startingNode;
	nodesToTest.emplace_back(currentNode);
	const std::pair<std::pair<int, int>, std::pair<int,int>> searchBounds = std::pair(
		std::pair(std::min(startingCoordinates.first, objectiveCoordinates.first) - 1, std::min(startingCoordinates.second, objectiveCoordinates.second) - 1),
		std::pair(std::max(startingCoordinates.first, startingCoordinates.first) + 1, std::max(startingCoordinates.second, objectiveCoordinates.second) + 1)
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
		modifiedNodes.emplace_back(currentNode);
		if (currentNode->neighbors.empty())
		{
			// Upper current neighbor
			if (currentNode->y-1 >= searchBounds.first.second)
			{
				currentNode->neighbors.emplace_back(&grid.getNodeAt(currentNode->x, currentNode->y - 1));
			}
			// Lower current neighbor
			if (currentNode->y+1 <= searchBounds.second.second)
			{
				currentNode->neighbors.emplace_back(&grid.getNodeAt(currentNode->x, currentNode->y + 1));
			}
			// Left current neighbor
			if (currentNode->x-1 >= searchBounds.first.first)
			{
				currentNode->neighbors.emplace_back(&grid.getNodeAt((currentNode->x - 1), currentNode->y));
			}
			// Right current neighbor
			if (currentNode->x+1 <= searchBounds.first.second)
			{
			currentNode->neighbors.emplace_back(&grid.getNodeAt((currentNode->x + 1), currentNode->y));
			}
		}

		
		// On teste tous les voisins
		for (auto* nodeNeighbor: currentNode->neighbors)
		{
			// Si le voisin n'est pas déjà visité et qu'il est navigable on l'ajoute aux nodes à tester
			if (!nodeNeighbor->visited && !nodeNeighbor->isOccupied())
			{
				nodesToTest.emplace_back(nodeNeighbor);
			}

			// Calcul du "coût" total potentiel pour naviguer à ce node voisin
			const float possiblyLowerGoal = currentNode->localGoal + getHeuristicDistance(currentNode, nodeNeighbor);
			// Si ce "coût" potentiel est inférieur à son "coût" actuel et donc que venir par ce node est le chemin le plus rapide pour arriver à ce node voisin
			if (possiblyLowerGoal < nodeNeighbor->localGoal)
			{
				// On met à jour le node voisin
				nodeNeighbor->parent = currentNode;
				nodeNeighbor->localGoal = possiblyLowerGoal;
				nodeNeighbor->globalGoal = nodeNeighbor->localGoal + getHeuristicDistance(nodeNeighbor, objectiveNode);
			}
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
	drawPath(grid, returnPath, searchBounds); //TODO remove debug
	for (auto* modifiedNode : modifiedNodes)
	{
		modifiedNode->resetNode();
	}
	mutex.unlock();
	return returnPath;
}

void NavigationSystem::drawPath(Grid& grid, const std::list<std::pair<int, int>>& path, const std::pair<std::pair<int, int>, std::pair<int, int>>&
                                bounds)
{
	std::cout << "Starting : [" << bounds.first.first << ";" << bounds.first.second << "]" << std::endl;
	std::cout << "Objective : [" << bounds.second.first << ";" << bounds.second.second << "]" << std::endl;
	for (int y = bounds.first.second; y < bounds.second.second; ++y)
	{
		for (int x = bounds.first.first; x < bounds.second.first; ++x)
		{
			int isInPath = 0;
			for (const auto& coordinate : path)
			{
				if (coordinate.first == x && coordinate.second == y)
				{
					isInPath = 1;
					break;
				}
				if(grid.isOccupied(x,y))
				{
					isInPath = 2;
				}
			}
			std::cout << isInPath;
		}
		std::cout << std::endl;
	}
}


float NavigationSystem::getHeuristicDistance(const Node* firstNode, const Node* secondNode)
{
	return sqrtf(static_cast<float>((firstNode->x - secondNode->x) * (firstNode->x - secondNode->x) + (firstNode->y - secondNode->y) * (firstNode->y - secondNode->y)));
}

#include "NavigationSystem.h"

#include <iostream>
#include <list>
#include <stack>

#include "Grid.h"

std::stack<std::pair<int, int>> NavigationSystem::aStarResolution(Grid& grid, const std::pair<int, int>& startingCoordinates, const std::pair<int, int>& objectiveCoordinates)
{
	const auto* objectiveNode = &grid.getNodeAt(objectiveCoordinates.first, objectiveCoordinates.second);
	auto* startingNode = &grid.getNodeAt(startingCoordinates.first, startingCoordinates.second);
	startingNode->localGoal = 0.0f;
	startingNode->globalGoal = getHeuristicDistance(startingNode, objectiveNode);
	std::list<Node*> nodesToTest;
	std::list<Node*> modifiedNodes;
	Node* currentNode = startingNode;
	nodesToTest.emplace_back(currentNode);
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
			currentNode->neighbors.emplace_back(&grid.getNodeAt(currentNode->x, currentNode->y - 1));
			// Lower current neighbor
			currentNode->neighbors.emplace_back(&grid.getNodeAt(currentNode->x, currentNode->y + 1));
			// Left current neighbor
			currentNode->neighbors.emplace_back(&grid.getNodeAt((currentNode->x - 1), currentNode->y));
			// Right current neighbor
			currentNode->neighbors.emplace_back(&grid.getNodeAt((currentNode->x + 1), currentNode->y));
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
			if (nodeNeighbor == objectiveNode)
			{
				nodesToTest.clear();
				break;
			}
		}
	}
	std::stack<std::pair<int,int>> returnPath;
	if (objectiveNode->parent) {
		auto* parent = objectiveNode->parent;
		while (parent != startingNode)
		{
			returnPath.emplace(std::pair(parent->x, parent->y));
			parent = parent->parent;
		}
	}
	for (auto* modifiedNode : modifiedNodes)
	{
		modifiedNode->resetNode();
	}
	return returnPath;
}


float NavigationSystem::getHeuristicDistance(const Node* firstNode, const Node* secondNode) {
	return sqrtf(static_cast<float>((firstNode->x - secondNode->x) * (firstNode->x - secondNode->x) + (firstNode->y - secondNode->y) * (firstNode->y - secondNode->y)));
}

void NavigationSystem::drawPath(Grid& grid, const std::pair<int, int>& startingCoordinates,
	const std::pair<int, int>& objectiveCoordinates)
{
	std::cout << "Starting : [" << startingCoordinates.first << ";" << startingCoordinates.second << "]" << std::endl;
	std::cout << "Objective : [" << objectiveCoordinates.first << ";" << objectiveCoordinates.second << "]" << std::endl;
	auto aStarResult = aStarResolution(grid, startingCoordinates, objectiveCoordinates);
	std::vector<std::pair<int,int>> vectorResult;
	vectorResult.resize(aStarResult.size());
	int i = 0;
	while (!aStarResult.empty())
	{
		vectorResult[i] = aStarResult.top();
		aStarResult.pop();
		++i;
	}
	for (int y = grid.getMinCoordinate().second; y < grid.getMaxCoordinate().second; ++y)
	{
		for (int x = grid.getMinCoordinate().first; x < grid.getMaxCoordinate().first; ++x)
		{
			bool isInPath = false;
			for (auto& coordinate : vectorResult)
			{
				if (coordinate.first == x && coordinate.second == y)
				{
					isInPath = true;
					break;
				}
			}
			std::cout << isInPath;
		}
		std::cout << std::endl;
	}
}

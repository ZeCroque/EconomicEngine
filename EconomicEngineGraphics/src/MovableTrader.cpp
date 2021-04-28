//
// Created by relin on 14/04/2021.
//

#include "MovableTrader.h"

#include <iostream>


#include "GameManager.h"
#include "NavigationSystem.h"
#include "Traders/Trader.h"

MovableTrader::MovableTrader(const std::string& inJobName, const std::string& inTextureName) : MovableActor(inTextureName)
{
	const std::hash<std::string> hasher;
	jobId = hasher(inJobName);
	pathFindSlotId = -1;
}

void MovableTrader::moveTo(Position inPosition)
{
	if (path.empty())
	{
		pathfindEndedSignal(false);
	} else
	{
		if (inPosition == Position::Workshop)
		{
			startPathfind(true);
		} else
		{
			startPathfind(false);
		}
	}
}

void MovableTrader::moveToRequestCallback(Trader* inTrader, Position inPosition)
{
	if (pathFindSlotId != -1)
	{
		pathfindEndedSignal.disconnect(pathFindSlotId);
	}
	pathFindSlotId = pathfindEndedSignal.connect([inTrader, this, inPosition](bool succeeded)
	 {
	     if (succeeded)
	     {
			inTrader->setPosition(inPosition);
	     } else
	     {
			inTrader->setPosition(inPosition == Position::Workshop ? Position::Market : Position::Workshop);
	     }
	 });
	moveTo(inPosition);
}

size_t MovableTrader::getJobId() const {
    return jobId;
}

MovableTrader* MovableTrader::clone()
{
	return new MovableTrader(*this);
}

void MovableTrader::calculatePathfind(const std::pair<int, int>& start, const std::pair<int, int>& end)
{
	path = NavigationSystem::aStarResolution(GameManager::getInstance()->getGridManager().getGrid(), start,end);
	for (int y = GameManager::getInstance()->getGridManager().getGrid().getMinCoordinate().second; y < GameManager::getInstance()->getGridManager().getGrid().getMaxCoordinate().second; ++y)
	{
		for (int x = GameManager::getInstance()->getGridManager().getGrid().getMinCoordinate().first; x < GameManager::getInstance()->getGridManager().getGrid().getMaxCoordinate().first; ++x)
		{
			int isInPath = 0;
			for (auto& coordinate : path)
			{
				if (coordinate.first == x && coordinate.second == y)
				{
					isInPath = 1;
					break;
				}
				if(GameManager::getInstance()->getGridManager().getGrid().isOccupied(x,y))
				{
					isInPath = 2;
				}
			}
			std::cout << isInPath;
		}
		std::cout << std::endl;
	}
}

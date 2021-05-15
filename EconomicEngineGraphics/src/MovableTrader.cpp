#include "MovableTrader.h"
#include "GameManager.h"
#include "NavigationSystem.h"

MovableTrader::MovableTrader(const std::string& inJobName, const std::string& inTextureName) : MovableActor(inTextureName)
{
	const std::hash<std::string> hasher;
	jobId = hasher(inJobName);
	pathFindSlotId = -1;
	boundTrader = nullptr;
}

void MovableTrader::calculatePathfind(const std::pair<int, int>& inStart, const std::pair<int, int>& inEnd)
{
	path = NavigationSystem::aStarResolution(GameManager::getInstance()->getGridManager().getGrid(), inStart,inEnd);
	path.emplace_front(std::pair<int,int>(inStart.first, inStart.second - 1));
	path.emplace_back(std::pair<int,int>(inEnd.first, inEnd.second - 1));
}

void MovableTrader::moveTo(const Position inPosition)
{
	if (path.empty())
	{
		pathfindEndedSignal(false);
	}
	else
	{
		if (inPosition == Position::Workshop)
		{
			startPathfind(true);
		}
		else
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
	pathFindSlotId = pathfindEndedSignal.connect([inTrader, this, inPosition](const bool bSucceeded)
	{
		if (bSucceeded)
		{
			inTrader->setPosition(inPosition);
		}
		else
		{
			inTrader->setPosition(inPosition == Position::Workshop ? Position::Market : Position::Workshop);
		}
	});
	moveTo(inPosition);
}

size_t MovableTrader::getJobId() const
{
    return jobId;
}

Trader* MovableTrader::getBoundTrader() const
{
    return boundTrader;
}

void MovableTrader::setBoundTrader(Trader* inBoundTrader)
{
    boundTrader = inBoundTrader;
}

MovableTrader* MovableTrader::clone()
{
	return new MovableTrader(*this);
}

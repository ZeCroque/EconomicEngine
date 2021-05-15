#include "Workshop.h"
#include "GameManager.h"
#include "MovableTrader.h"

Workshop::Workshop(const std::string &inName, const std::string &inJobName, const std::string &inTextureName) : StaticActor(inTextureName), closestMarketCoordinate(0, 0)
{
    name = inName;
    const std::hash<std::string> hash;
    id = hash(name);
    jobId = hash(inJobName);
}

bool Workshop::isAvailable() const
{
    return id && !getTrader();
}

const std::string &Workshop::getName() const
{
    return name;
}

size_t Workshop::getId() const
{
    return id;
}

size_t Workshop::getJobId() const
{
    return jobId;
}

MovableTrader* Workshop::getTrader() const
{
    const auto referencedTrader = trader.lock();
    return referencedTrader.get();
}

std::pair<int, int> Workshop::getClosestMarketCoordinate() const
{
    return closestMarketCoordinate;
}

void Workshop::setTrader(std::shared_ptr<MovableTrader> &inTrader)
{
    trader = inTrader;
}

void Workshop::setClosestMarketCoordinate(const std::pair<int, int>& inClosestMarketCoordinate)
{
	closestMarketCoordinate = inClosestMarketCoordinate;
	if (auto traderPtr = trader.lock(); traderPtr)
	{
		if (traderPtr->getDirection() != Direction::None)
		{
			traderPtr->getPathfindEndedSignal().connect([this, traderPtr](bool bSucceeded)
			{
				traderPtr->calculatePathfind(std::pair<int,int>(x, y + 1), std::pair<int,int>(closestMarketCoordinate.first, closestMarketCoordinate.second + 1));
			});
		}
		else
		{
			traderPtr->calculatePathfind(std::pair<int,int>(x, y + 1), std::pair<int,int>(closestMarketCoordinate.first, closestMarketCoordinate.second + 1));
		}
	}
}

Workshop* Workshop::clone()
{
    return new Workshop(*this);
}
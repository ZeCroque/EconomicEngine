//
// Created by relin on 14/04/2021.
//

#include "Workshop.h"


#include "GameManager.h"
#include "MovableTrader.h"


size_t Workshop::getJobId() const {
    return jobId;
}

Workshop::Workshop(const std::string &inName, const std::string &inJobName, const std::string &inTextureName)
        : StaticActor(inTextureName), closestMarketCoordinate(0, 0) {
    name = inName;
    const std::hash<std::string> hash;
    id = hash(name);
    jobId = hash(inJobName);
}

bool Workshop::isAvailable() const {
    return id && !getTrader();
}

const std::string &Workshop::getName() const {
    return name;
}

size_t Workshop::getId() const {
    return id;
}


void Workshop::setTrader(std::shared_ptr<MovableTrader> &inTrader) {
    trader = inTrader;
}

Workshop *Workshop::clone() {
    return new Workshop(*this);
}

MovableTrader *Workshop::getTrader() const {
    const auto referencedTrader = trader.lock();
    return referencedTrader.get();
}

void Workshop::setClosestMarketCoordinate(const std::pair<int, int> &inClosestMarketCoordinate) {
	closestMarketCoordinate = inClosestMarketCoordinate;
	if (auto traderPtr = trader.lock(); traderPtr)
	{
		if (traderPtr->getDirection() != Direction::None)
		{
			traderPtr->getPathfindEndedSignal().connect([this, traderPtr](bool succeeded)
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

std::pair<int, int> Workshop::getClosestMarketCoordinate()
{
    return closestMarketCoordinate;
}


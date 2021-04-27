//
// Created by relin on 14/04/2021.
//

#include "Workshop.h"


#include "GameManager.h"
#include "MovableTrader.h"
#include "NavigationSystem.h"


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
	//TODO if trader is moving then connect lambda to pathFindEnded signal to update its path afterward
	/*trader.lock()->calculatePathfind(,std::pair<int,int>(x, y + 1), std::pair<int,int>(inClosestMarketCoordinate.first, inClosestMarketCoordinate.second + 1));
    closestMarketCoordinate = inClosestMarketCoordinate;*/
		NavigationSystem::drawPath(GameManager::getInstance()->getGridManager().getGrid(), std::pair<int,int>(x, y + 1), std::pair<int,int>(inClosestMarketCoordinate.first, inClosestMarketCoordinate.second + 1));
    closestMarketCoordinate = inClosestMarketCoordinate;
	        //trader.lock()->startPathfind(false);
}


//
// Created by relin on 14/04/2021.
//

#include "MovableTrader.h"
#include "Traders/Trader.h"

MovableTrader::MovableTrader(const std::string &inJobName, const std::string &inTextureName) : MovableActor(inTextureName)
{
	movementSimulationThread = nullptr;
    std::hash<std::string> hasher;
    jobId = hasher(inJobName);
}

void MovableTrader::moveTo(Position position)
{
	pathfindSucceededSignal();
}

size_t MovableTrader::getJobId() const 
{
    return jobId;
}

MovableTrader *MovableTrader::clone()
{
    return new MovableTrader(*this);
}

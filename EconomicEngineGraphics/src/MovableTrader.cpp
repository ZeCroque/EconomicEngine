//
// Created by relin on 14/04/2021.
//

#include "MovableTrader.h"


#include "GameManager.h"
#include "NavigationSystem.h"
#include "Traders/Trader.h"

MovableTrader::MovableTrader(const std::string& inJobName, const std::string& inTextureName) : MovableActor(inTextureName)
{
	movementSimulationThread = nullptr;
	const std::hash<std::string> hasher;
	jobId = hasher(inJobName);
}

void MovableTrader::moveTo(Position position)
{
	movementSimulationThread = new std::thread([this]()
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>((24.f/6.f * 1000.f) / GameManager::getInstance()->getSpeedFactor())));
		pathfindSucceededSignal();
		pathfindSucceededSignal.disconnectAll();
	});
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
}

//
// Created by relin on 14/04/2021.
//

#include "MovableTrader.h"

#include "Traders/Trader.h"

MovableTrader::MovableTrader(const std::string& jobName) : movementSimulationThread(nullptr)
{
	std::hash<std::string> hasher;
	jobId = hasher(jobName);
}

void MovableTrader::moveTo(Position position)
{
	/*movementSimulationThread = new std::thread([this]()
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(24.f/12.f * 1000.f)));
		pathfindSucceededSignal();
	});
	movementSimulationThread->detach();*/
	pathfindSucceededSignal();
}

size_t MovableTrader::getJobId() const {
    return jobId;
}

MovableTrader* MovableTrader::clone()
{
	return new MovableTrader(*this);
}

//
// Created by relin on 14/04/2021.
//

#include "MovableTrader.h"

MovableTrader::MovableTrader(std::string jobName)
{
	std::hash<std::string> hasher;
	jobId = hasher(jobName);
}

size_t MovableTrader::getJobId() const {
    return jobId;
}

MovableTrader* MovableTrader::clone()
{
	return new MovableTrader(*this);
}

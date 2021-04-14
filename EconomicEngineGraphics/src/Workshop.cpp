//
// Created by relin on 14/04/2021.
//

#include "Workshop.h"


size_t Workshop::getJobId() const {
    return jobId;
}

Workshop::Workshop(std::string jobName)
{
	std::hash<std::string> hasher;
	jobId = hasher(jobName);
}

bool Workshop::isAvailable() const {
    return jobId && !getTrader();
}


void Workshop::setTrader(const std::shared_ptr<MovableTrader> &newTrader) {
    trader = newTrader;
}

Workshop* Workshop::clone()
{
	return new Workshop(*this);
}

MovableTrader *Workshop::getTrader() const {
    auto referencedTrader = trader.lock();
    return referencedTrader.get();
}


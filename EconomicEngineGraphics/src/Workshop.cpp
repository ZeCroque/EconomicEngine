//
// Created by relin on 14/04/2021.
//

#include "Workshop.h"


size_t Workshop::getJobId() const
{
    return jobId;
}

Workshop::Workshop(const std::string &inName, const std::string &inJobName, const std::string &inTextureName): StaticActor(inTextureName)
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


void Workshop::setTrader(const std::shared_ptr<MovableTrader> &inTrader) {
    trader = inTrader;
}

Workshop *Workshop::clone()
{
    return new Workshop(*this);
}

MovableTrader *Workshop::getTrader() const
{
    const auto referencedTrader = trader.lock();
    return referencedTrader.get();
}


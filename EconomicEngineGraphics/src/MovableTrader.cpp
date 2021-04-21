//
// Created by relin on 14/04/2021.
//

#include "MovableTrader.h"

MovableTrader::MovableTrader(const std::string &jobName, const std::string &textureName) : MovableActor(textureName)
{
    std::hash<std::string> hasher;
    jobId = hasher(jobName);
}

size_t MovableTrader::getJobId() const
{
    return jobId;
}

MovableTrader *MovableTrader::clone()
{
    return new MovableTrader(*this);
}

//
// Created by relin on 14/04/2021.
//

#include "Workshop.h"


size_t Workshop::getJobId() const {
    return jobId;
}

bool Workshop::isAvailable() const {
    return jobId && !getTrader();
}


void Workshop::setTrader(const std::shared_ptr<MovableTrader> &newTrader) {
    trader = newTrader;
}

MovableTrader *Workshop::getTrader() const {
    auto referencedTrader = trader.lock();
    return referencedTrader.get();
}

void Workshop::setJobId(size_t newJobId) {
    jobId = newJobId;
}


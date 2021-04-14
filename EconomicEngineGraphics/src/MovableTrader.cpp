//
// Created by relin on 14/04/2021.
//

#include "MovableTrader.h"

size_t MovableTrader::getJobId() const {
    return jobId;
}

void MovableTrader::setJobId(size_t newJobId) {
    jobId = newJobId;
}

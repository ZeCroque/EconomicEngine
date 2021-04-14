//
// Created by relin on 14/04/2021.
//

#ifndef ECONOMICENGINE_MOVABLETRADER_H
#define ECONOMICENGINE_MOVABLETRADER_H

#include "MovableActor.h"

class MovableTrader : public MovableActor {

public:
    void setJobId(size_t jobId);

    size_t getJobId() const;

private:
    size_t jobId;
};


#endif //ECONOMICENGINE_MOVABLETRADER_H

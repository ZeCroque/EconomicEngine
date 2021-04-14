//
// Created by relin on 14/04/2021.
//

#ifndef ECONOMICENGINE_WORKSHOP_H
#define ECONOMICENGINE_WORKSHOP_H

#include <any>
#include <Traders/Trader.h>
#include "StaticActor.h"

class MovableTrader;

class Workshop : public StaticActor {

public:
    bool isAvailable() const;

    size_t getJobId() const;

    MovableTrader *getTrader() const;

    void setJobId(size_t jobId);

    void setTrader(const std::shared_ptr<MovableTrader> &newTrader);

private:
    std::weak_ptr<MovableTrader> trader;
    size_t jobId;

};

#endif //ECONOMICENGINE_WORKSHOP_H

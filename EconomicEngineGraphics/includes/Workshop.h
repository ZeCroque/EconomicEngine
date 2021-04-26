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
    Workshop(const std::string& inName,const std::string& inJobName, const std::string& inTextureName);

    bool isAvailable() const;

	const std::string& getName() const;

    size_t getId() const;

    size_t getJobId() const;

    MovableTrader *getTrader() const;

    void setTrader(const std::shared_ptr<MovableTrader> &inTrader);

    void setClosestMarketCoordinate(const std::pair<int, int> &closestMarketCoordinate);

	Workshop* clone() override;

private:
    std::weak_ptr<MovableTrader> trader;
    std::pair<int, int> closestMarketCoordinate;

private:
    std::string name;
    size_t id;
    size_t jobId;
};

#endif //ECONOMICENGINE_WORKSHOP_H

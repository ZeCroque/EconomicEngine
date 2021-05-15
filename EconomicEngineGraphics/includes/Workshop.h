#ifndef WORKSHOP_H
#define WORKSHOP_H

#include "Traders/Trader.h"
#include "StaticActor.h"

class MovableTrader;

class Workshop : public StaticActor
{

public:
    Workshop(const std::string& inName,const std::string& inJobName, const std::string& inTextureName);

    bool isAvailable() const;

	const std::string& getName() const;

    size_t getId() const;

    size_t getJobId() const;

    MovableTrader* getTrader() const;

	std::pair<int, int> getClosestMarketCoordinate() const;

    void setTrader(std::shared_ptr<MovableTrader>& inTrader);

    void setClosestMarketCoordinate(const std::pair<int, int>& inClosestMarketCoordinate);

	Workshop* clone() override;

private:
    std::weak_ptr<MovableTrader> trader;
    std::pair<int, int> closestMarketCoordinate;
    std::string name;
    size_t id;
    size_t jobId;
};

#endif //WORKSHOP_H

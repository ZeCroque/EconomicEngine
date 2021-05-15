#ifndef MOVABLE_TRADER_H
#define MOVABLE_TRADER_H

#include <string>

#include "MovableActor.h"

enum class Position;

class Trader;

class MovableTrader : public MovableActor
{

public:
	MovableTrader(const std::string& inJobName, const std::string& inTextureName);

	void moveTo(Position inPosition);

	void moveToRequestCallback(Trader* inTrader, Position inPosition);

    size_t getJobId() const;

	MovableTrader* clone() override;
	void calculatePathfind(const std::pair<int, int>& inStart, const std::pair<int, int>& inEnd);

    Trader* getBoundTrader() const;
    void setBoundTrader(Trader* inBoundTrader);

private:
    size_t jobId;
    Trader* boundTrader;
    int pathFindSlotId;
};


#endif //MOVABLE_TRADER_H

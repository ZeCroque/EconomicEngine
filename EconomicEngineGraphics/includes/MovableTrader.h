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

	void calculatePathfind(const std::pair<int, int>& inStart, const std::pair<int, int>& inEnd);
	
	void moveTo(Position inPosition);

	void moveToRequestCallback(Trader* inTrader, Position inPosition);

    size_t getJobId() const;

	Trader* getBoundTrader() const;

    void setBoundTrader(Trader* inBoundTrader);

	MovableTrader* clone() override;

private:
    size_t jobId;
    Trader* boundTrader;
    int pathFindSlotId;
};


#endif //MOVABLE_TRADER_H

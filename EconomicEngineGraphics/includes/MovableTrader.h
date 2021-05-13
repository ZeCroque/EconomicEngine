//
// Created by relin on 14/04/2021.
//

#ifndef ECONOMICENGINE_MOVABLETRADER_H
#define ECONOMICENGINE_MOVABLETRADER_H

#include <string>
#include <thread>

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

    Trader *getBoundTrader() const;
    void setBoundTrader(Trader *boundTrader);

private:
    size_t jobId;
    Trader* boundTrader;
    int pathFindSlotId;
};


#endif //ECONOMICENGINE_MOVABLETRADER_H

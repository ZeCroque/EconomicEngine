//
// Created by relin on 14/04/2021.
//

#ifndef ECONOMICENGINE_MOVABLETRADER_H
#define ECONOMICENGINE_MOVABLETRADER_H

#include <string>
#include <thread>

#include "MovableActor.h"
enum class Position;

class MovableTrader : public MovableActor
{

public:
	MovableTrader(const std::string& inJobName, const std::string& inTextureName);

	void moveTo(Position inPosition);

	void moveToRequestCallback(class Trader* inTrader, Position inPosition);
	
    size_t getJobId() const;

	MovableTrader* clone() override;
	void calculatePathfind(const std::pair<int, int>& start, const std::pair<int, int>& end);

private:
    size_t jobId;

	int pathFindSlotId;
};


#endif //ECONOMICENGINE_MOVABLETRADER_H

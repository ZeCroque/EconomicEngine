//
// Created by relin on 14/04/2021.
//

#ifndef ECONOMICENGINE_MOVABLETRADER_H
#define ECONOMICENGINE_MOVABLETRADER_H

#include <string>
#include <thread>

#include "MovableActor.h"

class MovableTrader : public MovableActor
{

public:
	MovableTrader(const std::string& inJobName, const std::string& inTextureName);

	void moveTo(enum class Position position);
	
    size_t getJobId() const;

	MovableTrader* clone() override;

	//DEBUG
	std::thread* movementSimulationThread;

private:
    size_t jobId;
};


#endif //ECONOMICENGINE_MOVABLETRADER_H

//
// Created by relin on 14/04/2021.
//

#ifndef ECONOMICENGINE_MOVABLETRADER_H
#define ECONOMICENGINE_MOVABLETRADER_H

#include <string>

#include "MovableActor.h"

class MovableTrader : public MovableActor {

public:
	MovableTrader(const std::string& jobName, const std::string& textureName);

    size_t getJobId() const;

	MovableTrader* clone() override;

private:
    size_t jobId;
};


#endif //ECONOMICENGINE_MOVABLETRADER_H

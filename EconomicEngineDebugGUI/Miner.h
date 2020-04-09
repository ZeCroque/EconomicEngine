#ifndef MINER_H
#define MINER_H

#include "Job.h"

class Miner : public Job
{
public:
	Miner();
	Miner* clone() override { return new Miner(*this); }
};

#endif

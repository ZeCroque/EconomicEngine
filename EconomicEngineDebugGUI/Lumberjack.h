#ifndef LUMBERJACK_H
#define LUMBERJACK_H

#include "Job.h"

class Lumberjack : public Job
{
public:
	Lumberjack();
	Lumberjack* clone() override { return new Lumberjack(*this); }
};

#endif

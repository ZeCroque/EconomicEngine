#ifndef BUTCHER_H
#define BUTCHER_H

#include "Job.h"

class Butcher final : public Job
{
public:
	Butcher();
	Butcher* clone() override { return new Butcher(*this); }
};

#endif

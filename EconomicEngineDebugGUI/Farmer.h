#ifndef FARMER_H
#define FARMER_H

#include "Job.h"

class Farmer final : public Job
{
public:
	Farmer();
	Farmer* clone() override { return new Farmer(*this); }
};

#endif

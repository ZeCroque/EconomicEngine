#ifndef BAKER_H
#define BAKER_H

#include "Job.h"

class Baker final : public Job
{
public:
	Baker();
	Baker* clone() override { return new Baker(*this); }
};

#endif

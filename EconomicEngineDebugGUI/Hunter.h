#ifndef HUNTER_H
#define HUNTER_H

#include "Job.h"

class Hunter final : public Job
{
public:
	Hunter();
	Hunter* clone() override { return new Hunter(*this); }
};

#endif

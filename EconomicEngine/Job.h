#ifndef JOB_H
#define JOB_H

#include "CraftFactory.h"
#include "Craft.h"

class Job
{
public:
	Craft* craft(size_t typeId);
private:
	//CraftFactory craftsList;
};

#endif
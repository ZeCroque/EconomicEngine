#ifndef TOOLMAKER_H
#define TOOLMAKER_H

#include "Job.h"

class ToolMaker final : public Job
{
public:
	ToolMaker();
	ToolMaker* clone() override { return new ToolMaker(*this); }
};

#endif

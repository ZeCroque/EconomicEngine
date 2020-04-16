#ifndef AXE_BEHAVIOR_H
#define AXE_BEHAVIOR_H
#include "ToolBehavior.h"

class AxeBehavior : public ToolBehavior
{
public:
	AxeBehavior(const float craftRateBoost, const float degradationRate); 

	AxeBehavior* clone() override;

};

#endif

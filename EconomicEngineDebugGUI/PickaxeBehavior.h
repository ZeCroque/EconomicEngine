#ifndef PICKAXE_BEHAVIOR_H
#define PICKAXE_BEHAVIOR_H
#include "ToolBehavior.h"

class PickaxeBehavior : public ToolBehavior
{
public:
	PickaxeBehavior(const float craftRateBoost, const float degradationRate);
	PickaxeBehavior* clone() override;

};

#endif

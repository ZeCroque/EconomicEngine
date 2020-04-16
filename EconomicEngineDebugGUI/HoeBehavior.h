
#ifndef HOE_BEHAVIOR_H
#define HOE_BEHAVIOR_H
#include "ToolBehavior.h"

class HoeBehavior : public ToolBehavior
{
public:
	HoeBehavior(const float craftRateBoost, const float degradationRate);
	HoeBehavior* clone() override;

};

#endif

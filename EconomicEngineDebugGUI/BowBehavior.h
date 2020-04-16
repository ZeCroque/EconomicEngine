#ifndef BOW_BEHAVIOR_H
#define BOW_BEHAVIOR_H
#include "ToolBehavior.h"

class BowBehavior : public ToolBehavior
{
public:
	BowBehavior(const float craftRateBoost, const float degradationRate);

	BowBehavior* clone() override;

};

#endif

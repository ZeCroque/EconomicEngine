#ifndef SWORD_BEHAVIOR_H
#define SWORD_BEHAVIOR_H
#include "ToolBehavior.h"

class SwordBehavior : public ToolBehavior
{
public:
	SwordBehavior(const float craftRateBoost, const float degradationRate); 

	SwordBehavior* clone() override;

};

#endif

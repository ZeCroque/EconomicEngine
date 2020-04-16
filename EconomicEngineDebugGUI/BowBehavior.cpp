#include "BowBehavior.h"
#include <typeinfo>

BowBehavior::BowBehavior(const float craftRateBoost, const float degradationRate) : ToolBehavior(craftRateBoost, degradationRate)
{
	this->typeId = typeid(BowBehavior).hash_code();
}

BowBehavior* BowBehavior::clone()
{
	return new BowBehavior(*this);
}
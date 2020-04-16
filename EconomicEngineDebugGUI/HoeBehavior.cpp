#include "HoeBehavior.h"
#include <typeinfo>

HoeBehavior::HoeBehavior(const float craftRateBoost, const float degradationRate) : ToolBehavior(craftRateBoost, degradationRate)
{
	this->typeId = typeid(HoeBehavior).hash_code();
}

HoeBehavior* HoeBehavior::clone()
{
	return new HoeBehavior(*this);
}

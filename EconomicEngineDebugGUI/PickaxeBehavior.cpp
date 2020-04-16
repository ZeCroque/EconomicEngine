#include "PickaxeBehavior.h"
#include <typeinfo>

PickaxeBehavior::PickaxeBehavior(const float craftRateBoost, const float degradationRate) : ToolBehavior(craftRateBoost, degradationRate)
{
	this->typeId = typeid(PickaxeBehavior).hash_code();
}

PickaxeBehavior* PickaxeBehavior::clone()
{
	return new PickaxeBehavior(*this);
}


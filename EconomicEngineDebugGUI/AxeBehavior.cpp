#include "AxeBehavior.h"
#include <typeinfo>

AxeBehavior::AxeBehavior(const float craftRateBoost, const float degradationRate) : ToolBehavior(craftRateBoost, degradationRate)
{
	this->typeId = typeid(AxeBehavior).hash_code();
}

AxeBehavior* AxeBehavior::clone()
{
	return new AxeBehavior(*this);
}

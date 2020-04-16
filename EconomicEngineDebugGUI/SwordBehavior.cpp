#include "SwordBehavior.h"
#include <typeinfo>

SwordBehavior::SwordBehavior(const float craftRateBoost, const float degradationRate) : ToolBehavior(craftRateBoost, degradationRate)
{
	this->typeId = typeid(SwordBehavior).hash_code();
}

SwordBehavior* SwordBehavior::clone()
{
	return new SwordBehavior(*this);
}

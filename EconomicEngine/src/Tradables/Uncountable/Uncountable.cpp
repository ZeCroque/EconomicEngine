#include "Tradables/Uncountable/Uncountable.h"

Uncountable::Uncountable() : Tradable()
{
	behavior = nullptr;
}

Uncountable::Uncountable(const std::string& inName, const std::pair<float, float> inDefaultPriceBelief, Behavior* inBehavior) : Tradable(inName, inDefaultPriceBelief)
{
	behavior = inBehavior;
	behavior->init(this);
}

Uncountable::Uncountable(const Uncountable& inUncountable) : Tradable(inUncountable)
{
	if (inUncountable.behavior)
	{
		behavior = inUncountable.behavior->clone();
		behavior->init(this);
	}
}

Uncountable::~Uncountable()
{
	delete behavior;
}

Behavior* Uncountable::getBehavior() const
{
	return behavior;
}

Uncountable* Uncountable::clone()
{
	return new Uncountable(*this);
}

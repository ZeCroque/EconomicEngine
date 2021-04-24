#include "Tradables/Uncountable/Uncountable.h"

Uncountable::Uncountable() : Tradable()
{
	behavior = nullptr;
}

Uncountable::Uncountable(const std::string inName, std::pair<float, float> inDefaultPriceBelief, Behavior* inBehavior) : Tradable(inName, inDefaultPriceBelief)
{
	behavior = inBehavior;
	behavior->init(this);
}

Uncountable::Uncountable(const Uncountable& uncountable) : Tradable(uncountable)
{
	if (uncountable.behavior != nullptr)
	{
		behavior = uncountable.behavior->clone();
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

void Uncountable::setOwningTrader(Trader* inOwningTrader)
{
	Tradable::setOwningTrader(inOwningTrader);
	
}

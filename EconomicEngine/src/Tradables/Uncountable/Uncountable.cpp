#include "Tradables/Uncountable/Uncountable.h"

Uncountable::Uncountable() : Tradable()
{
	behavior = nullptr;
}

Uncountable::Uncountable(std::string newName, std::pair<float, float> newDefaultPriceBelief, Behavior* newBehavior) : Tradable(newName, newDefaultPriceBelief)
{
	behavior = newBehavior;
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

void Uncountable::setOwner(Trader* newOwner)
{
	Tradable::setOwner(newOwner);
	
}

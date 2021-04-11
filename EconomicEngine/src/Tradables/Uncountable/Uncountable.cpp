#include "Tradables/Uncountable/Uncountable.h"

Uncountable::Uncountable() : Tradable()
{
	this->behavior = nullptr;
}

Uncountable::Uncountable(std::string name, std::pair<float, float> defaultPriceBelief, Behavior* behavior) : Tradable(name, defaultPriceBelief)
{
	this->behavior = behavior;
}

Uncountable::Uncountable(const Uncountable& uncountable) : Tradable(uncountable)
{
	this->behavior = nullptr;
	if (uncountable.behavior != nullptr)
	{
		this->behavior = uncountable.behavior->clone();
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

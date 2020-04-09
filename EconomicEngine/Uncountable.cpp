#include "Uncountable.h"

Uncountable::Uncountable() : Tradable()
{
	this->behavior = nullptr;
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

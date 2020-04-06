#include "Tradable.h"

Tradable::Tradable() : behavior(nullptr) { }

Tradable::Tradable(const Tradable& tradable)
{
	this->name = tradable.name;
	this->behavior = new Behavior(*tradable.behavior);
}

std::string Tradable::getName() const
{
	return name;
}

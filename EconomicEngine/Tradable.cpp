#include "Tradable.h"
#include <typeinfo>

Tradable::Tradable() : behavior(nullptr), id(0) { }

Tradable::Tradable(const Tradable& tradable) : Tradable()
{
	this->name = tradable.name;
	this->behavior = new Behavior(*tradable.behavior);
}

std::string Tradable::getName() const
{
	return name;
}

size_t Tradable::getId() const
{
	return this->id;
}

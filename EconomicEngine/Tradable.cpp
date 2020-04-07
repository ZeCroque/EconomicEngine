#include "Tradable.h"
#include <typeinfo>

Tradable::Tradable() : id(0), behavior(nullptr){}

Tradable::Tradable(const Tradable& tradable) : Tradable()
{
	this->id = tradable.id;
	this->name = std::string(tradable.name);
	this->defaultPriceBelief = std::pair<float, float>(tradable.defaultPriceBelief);
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

std::pair<float, float> Tradable::getDefaultPriceBelief() const
{
	return this->defaultPriceBelief;
}

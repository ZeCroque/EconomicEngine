#include "Tradable.h"

Tradable::Tradable() : id(0){}

Tradable::Tradable(const Tradable& tradable) : Tradable()
{
	this->id = tradable.id;
	this->name = std::string(tradable.name);
	this->defaultPriceBelief = std::pair<float, float>(tradable.defaultPriceBelief);
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

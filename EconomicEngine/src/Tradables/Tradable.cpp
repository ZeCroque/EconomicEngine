#include "Tradables/Tradable.h"

#include "Traders/Trader.h"

Tradable::Tradable() : id(0), owningTrader(nullptr){}

Tradable::Tradable(std::string newName, std::pair<float, float> newDefaultPriceBelief) : name(std::move(newName)), defaultPriceBelief(std::move(newDefaultPriceBelief)), owningTrader(nullptr)
{
	const std::hash<std::string> hasher;
	id = hasher(this->name);
}

Tradable::Tradable(const Tradable& tradable) : Tradable()
{
	id = tradable.id;
	name = std::string(tradable.name);
	defaultPriceBelief = std::pair<float, float>(tradable.defaultPriceBelief);
}

void Tradable::setOwner(Trader* newOwningTrader)
{
	owningTrader = newOwningTrader;
}

void Tradable::removeFromOwnerInventory()
{
	owningTrader->removeFromInventory(this);
}

std::string Tradable::getName() const
{
	return name;
}

Trader* Tradable::getOwningTrader() const
{
	return owningTrader;
}

size_t Tradable::getId() const
{
	return id;
}


const std::pair<float, float>& Tradable::getDefaultPriceBelief() const
{
	return defaultPriceBelief;
}

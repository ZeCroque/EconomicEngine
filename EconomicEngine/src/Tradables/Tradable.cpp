#include "Tradables/Tradable.h"
#include "Traders/Trader.h"

Tradable::Tradable() : id(0), owningTrader(nullptr)
{
}

Tradable::Tradable(std::string inName, std::pair<float, float> inDefaultPriceBelief) : name(std::move(inName)), defaultPriceBelief(std::move(inDefaultPriceBelief)), owningTrader(nullptr)
{
	const std::hash<std::string> hasher;
	id = hasher(name);
}

Tradable::Tradable(const Tradable& inTradable) : Tradable()
{
	id = inTradable.id;
	name = std::string(inTradable.name);
	defaultPriceBelief = std::pair<float, float>(inTradable.defaultPriceBelief);
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

void Tradable::setOwningTrader(Trader* inOwningTrader)
{
	owningTrader = inOwningTrader;
}
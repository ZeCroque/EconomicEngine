#include "Tradables/Countable.h"

Countable::Countable() : Tradable(), count(1)
{
}

Countable::Countable(const Countable& inCountable) : Tradable(inCountable)
{
	count = inCountable.count;
}

Countable::Countable(const std::string& inName, const std::pair<float, float> inDefaultPriceBelief, const int inCount) : Tradable(inName, inDefaultPriceBelief), count(inCount)
{
}

int Countable::getCount() const
{
	return count;
}

void Countable::setCount(const int inCount)
{
	count = inCount;
}

void Countable::incrementCountBy(const int inAmount)
{
	count += inAmount;
}

void Countable::decrementCountBy(const int inAmount)
{
	count -= inAmount;
}

Countable* Countable::clone()
{
	return new Countable(*this);
}

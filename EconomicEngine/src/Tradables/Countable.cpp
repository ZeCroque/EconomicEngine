#include "Tradables/Countable.h"

Countable::Countable() : Tradable(), count(1){}

Countable::Countable(const Countable& inCountable) : Tradable(inCountable)
{
	count = inCountable.count;
}

Countable::Countable(const std::string& inName, const std::pair<float, float> inDefaultPriceBelief, const int inCount) : Tradable(inName, inDefaultPriceBelief), count(inCount){}

void Countable::setCount(const int inCount)
{
	count = inCount;
}

void Countable::incrementCountBy(const int inCount)
{
	count += inCount;
}

void Countable::decrementCountBy(const int inCount)
{
	count -= inCount;
}

int Countable::getCount() const
{
	return count;
}

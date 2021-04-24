#include "Tradables/Countable.h"

Countable::Countable() : Tradable(), count(1){}

Countable::Countable(const Countable& countable) : Tradable(countable)
{
	this->count = countable.count;
}

Countable::Countable(const std::string& inName, const std::pair<float, float> inDefaultPriceBelief, const int inCount) : Tradable(inName, inDefaultPriceBelief), count(inCount){}

void Countable::setCount(const int inCount)
{
	this->count = inCount;
}

void Countable::incrementCountBy(const int inCount)
{
	this->count += inCount;
}

void Countable::decrementCountBy(const int inCount)
{
	this->count -= inCount;
}

int Countable::getCount() const
{
	return count;
}

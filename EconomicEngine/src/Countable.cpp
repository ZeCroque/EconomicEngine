#include "Countable.h"

Countable::Countable() : Tradable(), count(1){}

Countable::Countable(const Countable& countable) : Tradable(countable)
{
	this->count = countable.count;
}

Countable::Countable(std::string name, std::pair<float, float> defaultPriceBelief, const int count) : Tradable(name, defaultPriceBelief), count(count){}

void Countable::setCount(const int count)
{
	this->count = count;
}

void Countable::incrementCountBy(const int count)
{
	this->count += count;
}

void Countable::decrementCountBy(const int count)
{
	this->count -= count;
}

int Countable::getCount() const
{
	return count;
}

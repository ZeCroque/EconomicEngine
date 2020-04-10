#include "Countable.h"

Countable::Countable() : Tradable(), count(1){}

Countable::Countable(const Countable& countable) : Tradable(countable)
{
	this->count = countable.count;
}

Countable::Countable(const int count) : Tradable(), count(count){}

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

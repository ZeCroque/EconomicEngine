#include "Countable.h"
#include <typeinfo>

Countable::Countable() : Tradable(), count(1){}

Countable::Countable(const Countable& countable) : Tradable(countable)
{
	this->count = countable.count;
}

Countable::Countable(const float count) : Tradable(), count(count){}

void Countable::incrementCountBy(const float count)
{
	this->count += count;
}

void Countable::decrementCountBy(const float count)
{
	this->count -= count;
}

float Countable::getCount() const
{
	return count;
}

#include "Countable.h"

Countable::Countable() : Tradable(), count(0) {}

Countable::Countable(const Countable& countable) : Tradable(countable)
{
	this->count = countable.count;
}

Countable::Countable(const int count) : Tradable(), count(count) {}

void Countable::incrementCountBy(const int count)
{
	this->count += count;
}

int Countable::getCount() const
{
	return count;
}

Wheat* Wheat::clone()
{
	return new Wheat(*this);
}

Bread* Bread::clone()
{
	return new Bread(*this);
}

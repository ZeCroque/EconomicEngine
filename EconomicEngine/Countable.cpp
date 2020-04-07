#include "Countable.h"
#include <typeinfo>

Countable::Countable() : Tradable(), count(1) {}

Countable::Countable(const Countable& countable) : Tradable(countable)
{
	this->count = countable.count;
}

Countable::Countable(const int count) : Tradable(), count(count) {}

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

Wheat::Wheat() : Countable() {}

Wheat::Wheat(const Wheat& wheat) : Countable(wheat)
{
	this->id = typeid(Wheat).hash_code();
}

Wheat* Wheat::clone()
{
	return new Wheat(*this);
}

Bread::Bread() : Countable(){}

Bread::Bread(const Bread& bread) : Countable(bread)
{
	this->id = typeid(Bread).hash_code();
}

Bread* Bread::clone()
{
	return new Bread(*this);
}

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

Gold::Gold() : Countable()
{
	this->id = typeid(Gold).hash_code();
	this->count = 1;
	this->name = "Or";
	this->defaultPriceBelief = std::pair<float, float>(10.0f, 50.0f);
}

Gold* Gold::clone()
{
	return new Gold(*this);
}

GoldenBread::GoldenBread() : Countable()
{
	this->id = typeid(GoldenBread).hash_code();
	this->count = 1;
	this->name = "Pain goldé";
	this->defaultPriceBelief = std::pair<float, float>(100.0f, 150.0f);
}

GoldenBread* GoldenBread::clone()
{
	return new GoldenBread(*this);
}

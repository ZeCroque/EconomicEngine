#include "Bow.h"

#include <typeinfo>

#include "BowBehavior.h"


Bow::Bow() : Uncountable()
{
	this->id = typeid(Bow).hash_code();
	this->name = "Bow";
	this->defaultPriceBelief = std::pair<float, float>(5.0f, 7.0f);
	this->behavior = new BowBehavior(0.33f, 0.1f);
}

Bow* Bow::clone()
{
	return new Bow(*this);
}

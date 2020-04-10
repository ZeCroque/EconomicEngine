#include "Bow.h"

Bow::Bow() : Uncountable()
{
	this->id = typeid(Bow).hash_code();
	this->name = "Bow";
	this->defaultPriceBelief = std::pair<float, float>(10.0f, 15.0f);
	this->behavior = new ToolBehavior(0.33f, 0.1f);
}

Bow* Bow::clone()
{
	return new Bow(*this);
}

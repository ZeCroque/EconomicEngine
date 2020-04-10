#include "Axe.h"

Axe::Axe() : Uncountable()
{
	this->id = typeid(Axe).hash_code();
	this->name = "Axe";
	this->defaultPriceBelief = std::pair<float, float>(10.0f, 15.0f);
	this->behavior = new ToolBehavior(0.33f, 0.1f);
}

Axe* Axe::clone()
{
	return new Axe(*this);
}

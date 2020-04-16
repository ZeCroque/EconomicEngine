#include "Axe.h"

#include "AxeBehavior.h"

Axe::Axe() : Uncountable()
{
	this->id = typeid(Axe).hash_code();
	this->name = "Axe";
	this->defaultPriceBelief = std::pair<float, float>(5.0f, 7.0f);
	this->behavior = new AxeBehavior(0.33f, 0.1f);
}

Axe* Axe::clone()
{
	return new Axe(*this);
}

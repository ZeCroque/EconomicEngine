#include "Pickaxe.h"

#include "PickaxeBehavior.h"

Pickaxe::Pickaxe() : Uncountable()
{
	this->id = typeid(Pickaxe).hash_code();
	this->name = "Pickaxe";
	this->defaultPriceBelief = std::pair<float, float>(5.0f, 7.0f);
	this->behavior = new PickaxeBehavior(0.33f, 0.1f);
}

Pickaxe* Pickaxe::clone()
{
	return new Pickaxe(*this);
}

#include "Pickaxe.h"

Pickaxe::Pickaxe() : Uncountable()
{
	this->id = typeid(Pickaxe).hash_code();
	this->name = "Pickaxe";
	this->defaultPriceBelief = std::pair<float, float>(10.0f, 15.0f);
	this->behavior = new ToolBehavior(0.33f, 0.1f);
}

Pickaxe* Pickaxe::clone()
{
	return new Pickaxe(*this);
}
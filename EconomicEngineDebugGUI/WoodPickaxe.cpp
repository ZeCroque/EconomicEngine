#include "WoodPickaxe.h"

#include "PickaxeBehavior.h"

WoodPickaxe::WoodPickaxe() : Uncountable()
{
	this->id = typeid(WoodPickaxe).hash_code();
	this->name = "Wood Pickaxe";
	this->defaultPriceBelief = std::pair<float, float>(3.0f, 5.0f);
	this->behavior = new PickaxeBehavior(0.0f, 0.1f);
}

WoodPickaxe* WoodPickaxe::clone()
{
	return new WoodPickaxe(*this);
}

#include "WoodPickaxe.h"

WoodPickaxe::WoodPickaxe() : Uncountable()
{
	this->id = typeid(WoodPickaxe).hash_code();
	this->name = "Wood Pickaxe";
	this->defaultPriceBelief = std::pair<float, float>(4.0f, 7.0f);
	this->behavior = new ToolBehavior(0.0f, 0.1f);
}

WoodPickaxe* WoodPickaxe::clone()
{
	return new WoodPickaxe(*this);
}

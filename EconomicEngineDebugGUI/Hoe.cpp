#include "Hoe.h"

Hoe::Hoe() : Uncountable()
{
	this->id = typeid(Hoe).hash_code();
	this->name = "Hoe";
	this->defaultPriceBelief = std::pair<float, float>(5.0f, 7.0f);
	this->behavior = new ToolBehavior(0.33f, 0.1f);
}

Hoe* Hoe::clone()
{
	return new Hoe(*this);
}

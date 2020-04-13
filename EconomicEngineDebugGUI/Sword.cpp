#include "Sword.h"

Sword::Sword() : Uncountable()
{
	this->id = typeid(Sword).hash_code();
	this->name = "Sword";
	this->defaultPriceBelief = std::pair<float, float>(7.0f, 10.0f);
	this->behavior = new ToolBehavior(0.33f, 0.1f);
}

Sword* Sword::clone()
{
	return new Sword(*this);
}

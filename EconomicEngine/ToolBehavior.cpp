#include "ToolBehavior.h"
#include "Trader.h"

ToolBehavior::ToolBehavior() : Behavior(), craftRateBoost(0.0f), degradationRate(0.0f), durability(1.0f){}

ToolBehavior::ToolBehavior(const float craftRateBoost, const float degradationRate) : ToolBehavior()
{
	this->craftRateBoost = craftRateBoost;
	this->degradationRate = degradationRate;
}

void ToolBehavior::init(Trader* owner, Tradable* item)
{
	this->owner = owner;
	this->item = item;
	this->owner->addObserver(this);
}

void ToolBehavior::notify(Observable* sender)
{
	if (sender == owner)
	{
		owner->getCurrentCraft()->addObserver(this);
		owner->getCurrentCraft()->incrementRate(craftRateBoost);
	}
	else
	{
		durability -= degradationRate;
		if(durability <= 0.0f)
		{
			owner->removeObserver(this);
			owner->removeFromInventory(item);
		}
	}
}

ToolBehavior* ToolBehavior::clone()
{
	return new ToolBehavior(*this);
}

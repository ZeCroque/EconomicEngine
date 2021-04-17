#include "Tradables/Uncountable/ToolBehavior.h"

#include "Tradables/Uncountable/Uncountable.h"

ToolBehavior::ToolBehavior() : Behavior(), craftRateBoost(0.0f), degradationRate(0.0f), durability(1.0f){}

ToolBehavior::ToolBehavior(const float newCraftRateBoost, const float newDegradationRate) : craftRateBoost(newCraftRateBoost), degradationRate(newDegradationRate), durability(1.0f){}

void ToolBehavior::init(Uncountable* newOwningTool)
{
	owningTool = newOwningTool;
}

void ToolBehavior::updateToolDurability()
{
	durability -= degradationRate;
	if(durability<=0)
	{
		owningTool->removeFromOwnerInventory();
	}
}

float ToolBehavior::getCraftRateBoost() const
{
	return craftRateBoost;
}

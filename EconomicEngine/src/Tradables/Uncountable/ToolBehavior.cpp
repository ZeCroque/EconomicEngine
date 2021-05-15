#include "Tradables/Uncountable/ToolBehavior.h"
#include "Tradables/Uncountable/Uncountable.h"

ToolBehavior::ToolBehavior() : Behavior(), craftRateBoost(0.0f), degradationRate(0.0f), durability(1.0f){}

ToolBehavior::ToolBehavior(const std::string& inName, const float inCraftRateBoost, const float inDegradationRate) : Behavior(inName)
{
	craftRateBoost = inCraftRateBoost;
	degradationRate = inDegradationRate;
	durability = 1.0f;
}

void ToolBehavior::init(Uncountable* inOwningTool)
{
	owningTool = inOwningTool;
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

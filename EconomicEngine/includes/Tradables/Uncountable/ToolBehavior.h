#ifndef	TOOL_BEHAVIOR_H
#define TOOL_BEHAVIOR_H

#include "Behavior.h"

class Craft;

class ToolBehavior : public Behavior
{
public:
	ToolBehavior();
	
	ToolBehavior(const std::string& inName, float inCraftRateBoost, float inDegradationRate);
	
	void init(Uncountable* inOwningTool) override;

	void updateDurability();
	
	[[nodiscard]] float getCraftRateBoost() const;

	ToolBehavior* clone() override;

private:
	float craftRateBoost;
	float degradationRate;
	float durability;
};

#endif //TOOL_BEHAVIOR_H
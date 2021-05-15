#ifndef	TOOL_BEHAVIOR_H
#define TOOL_BEHAVIOR_H

#include "Behavior.h"

class Craft;

class ToolBehavior : public Behavior
{
protected:
	float craftRateBoost;
	float degradationRate;
	float durability;
	ToolBehavior();
public:
	ToolBehavior(const std::string& inName, float inCraftRateBoost, float inDegradationRate);
	void init(Uncountable* inOwningTool) override;

	void updateToolDurability();
	
	[[nodiscard]] float getCraftRateBoost() const;

	ToolBehavior* clone() override { return new ToolBehavior(*this); }
};

#endif //TOOL_BEHAVIOR_H
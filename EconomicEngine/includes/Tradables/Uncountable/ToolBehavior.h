#ifndef	TOOL_BEHAVIOR
#define TOOL_BEHAVIOR

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
	ToolBehavior(float inCraftRateBoost, float inDegradationRate);
	void init(Uncountable* inOwningTool) override;

	void updateToolDurability();
	
	[[nodiscard]] float getCraftRateBoost() const;

	ToolBehavior* clone() override { return new ToolBehavior(*this); }
};

#endif
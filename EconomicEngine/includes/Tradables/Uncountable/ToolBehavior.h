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
	ToolBehavior(float craftRateBoost, float degradationRate);
	void init(Trader* owner, Tradable* item) override;
	void notify(Observable* sender) override;
	[[nodiscard]] float getCraftRateBoost() const;

	ToolBehavior* clone() override { return new ToolBehavior(*this); }
};

#endif
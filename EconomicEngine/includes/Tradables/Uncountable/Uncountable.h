#ifndef UNCOUNTABLE_H
#define UNCOUNTABLE_H

#include "Tradables/Tradable.h"
#include "ToolBehavior.h"

class Uncountable : public Tradable
{
protected:
	Behavior* behavior;

public:
	Uncountable();
	Uncountable(const std::string& inName, std::pair<float, float> inDefaultPriceBelief, Behavior* inBehavior);
	Uncountable(const Uncountable& inUncountable);
	~Uncountable() override;
	[[nodiscard]] Behavior* getBehavior() const;

	void setOwningTrader(Trader* inOwningTrader) override;

	Uncountable* clone() override { return new Uncountable(*this); }
};

#endif //UNCOUNTABLE_H
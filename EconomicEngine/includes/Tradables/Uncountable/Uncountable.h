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
	Uncountable(std::string newName, std::pair<float, float> newDefaultPriceBelief, Behavior* newBehavior);
	Uncountable(const Uncountable& uncountable);
	~Uncountable() override;
	[[nodiscard]] Behavior* getBehavior() const;

	void setOwner(Trader* newOwner) override;

	Uncountable* clone() override { return new Uncountable(*this); }
};

#endif
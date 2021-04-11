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
	Uncountable(std::string name, std::pair<float, float> defaultPriceBelief, Behavior* behavior);
	Uncountable(const Uncountable& uncountable);
	~Uncountable() override;
	[[nodiscard]] Behavior* getBehavior() const;

	Uncountable* clone() override { return new Uncountable(*this); }
};

#endif
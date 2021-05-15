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

	Uncountable* clone() override;
};

#endif //UNCOUNTABLE_H
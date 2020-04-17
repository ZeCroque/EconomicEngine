#ifndef UNCOUNTABLE_H
#define UNCOUNTABLE_H
#include "Tradable.h"

#include "ToolBehavior.h"

class Uncountable : public Tradable
{
protected:
	Behavior* behavior;

	Uncountable();

public:
	Uncountable(const Uncountable& uncountable);
	~Uncountable();
	[[nodiscard]] Behavior* getBehavior() const;
};

#endif
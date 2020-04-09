#ifndef UNCOUNTABLE_H
#define UNCOUNTABLE_H
#include "Tradable.h"
#include <typeinfo>

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

class Hoe final : public Uncountable
{
public:
	Hoe() : Uncountable()
	{
		this->id = typeid(Hoe).hash_code();
		this->name = "Hoe";
		this->defaultPriceBelief = std::pair<float, float>(10.0f, 15.0f);
		this->behavior = new ToolBehavior(0.2f, 0.1f);
	}

	Hoe(const Hoe&) = default;
	
	Hoe* clone() override
	{
		return new Hoe(*this);
	}
};
#endif
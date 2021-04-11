#ifndef COUNTABLE_H
#define COUNTABLE_H

#include "Tradable.h"

class Countable : public Tradable
{
protected:
	int count;

public:
	Countable();
	Countable(const Countable& countable);
	Countable(std::string name, std::pair<float, float> defaultPriceBelief, int count = 1);
	void setCount(int count);
	void incrementCountBy(int count);
	void decrementCountBy(int count);
	[[nodiscard]] int getCount() const;

	Countable* clone() override { return new Countable(*this); }
};

#endif


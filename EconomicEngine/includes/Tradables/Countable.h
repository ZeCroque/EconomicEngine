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
	Countable(const std::string& inName, std::pair<float, float> inDefaultPriceBelief, int inCount = 1);
	void setCount(int inCount);
	void incrementCountBy(int inCount);
	void decrementCountBy(int inCount);
	[[nodiscard]] int getCount() const;

	Countable* clone() override { return new Countable(*this); }
};

#endif


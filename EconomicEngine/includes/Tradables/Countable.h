#ifndef COUNTABLE_H
#define COUNTABLE_H

#include "Tradable.h"

class Countable : public Tradable
{
protected:
	int count;

public:
	Countable();
	
	Countable(const Countable& inCountable);
	
	Countable(const std::string& inName, std::pair<float, float> inDefaultPriceBelief, int inCount = 1);

	[[nodiscard]] int getCount() const;
	
	void setCount(int inCount);
	
	void incrementCountBy(int inAmount);
	
	void decrementCountBy(int inAmount);

	Countable* clone() override;
};

#endif //COUNTABLE_H


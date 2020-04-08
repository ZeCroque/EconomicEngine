#ifndef COUNTABLE_H
#define COUNTABLE_H

#include "Tradable.h"

class Countable : public Tradable
{
protected:
	float count;

public:
	Countable();
	Countable(const Countable& countable);
	Countable(float count);
	void incrementCountBy(float count);
	void decrementCountBy(float count);
	[[nodiscard]] float getCount() const;
};

#endif

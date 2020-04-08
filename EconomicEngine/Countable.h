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
	Countable(int count);
	void incrementCountBy(int count);
	void decrementCountBy(int count);
	[[nodiscard]] int getCount() const;
};

#endif

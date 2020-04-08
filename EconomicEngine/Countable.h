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

//DEBUG

class Gold : public Countable
{
public:
	Gold();
	Gold(const Gold& gold) = default;
	Gold* clone() override;
};

class GoldenBread : public Countable
{
public:
	GoldenBread();
	GoldenBread(const GoldenBread& goldenBread) = default;
	GoldenBread* clone() override;
};

#endif


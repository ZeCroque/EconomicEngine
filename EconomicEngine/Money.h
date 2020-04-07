#ifndef MONEY_H
#define MONEY_H

#include "Countable.h"

class Money : public Countable
{
public:
	Money();
	Money(float count);
	Money* clone() override;
};

#endif
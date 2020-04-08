#ifndef MEAT_H
#define MEAT_H

#include "Countable.h"

class Meat final : public Countable
{
public:
	Meat();
	Meat(const Meat& meat);
	Meat* clone() override;
};

#endif

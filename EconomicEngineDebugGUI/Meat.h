#ifndef MEAT_H
#define MEAT_H

#include "Countable.h"

class Meat final : public Countable
{
public:
	Meat();
	Meat(const Meat&) = default;
	Meat* clone() override;
};

#endif

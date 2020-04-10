#ifndef SWORD_H
#define SWORD_H

#include "Uncountable.h"

class Sword final : public Uncountable
{
public:
	Sword();
	Sword(const Sword&) = default;
	Sword* clone() override;
};
#endif

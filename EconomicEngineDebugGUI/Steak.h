#ifndef STEAK_H
#define STEAK_H

#include "Food.h"

class Steak final : public Food
{
public:
	Steak();
	Steak(const Steak&) = default;
	Steak* clone() override;
};

#endif

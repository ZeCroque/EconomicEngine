#ifndef STEAK_H
#define STEAK_H

#include <string>

#include "Food.h"


class Steak final : public Food
{
public:
	Steak();
	Steak(const Steak& steak);
	Steak* clone() override;
};

#endif

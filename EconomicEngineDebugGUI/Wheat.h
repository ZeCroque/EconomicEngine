#ifndef WHEAT_H
#define WHEAT_H

#include "Countable.h"

class Wheat final : public Countable
{
public:
	Wheat();
	Wheat(const Wheat&) = default;
	Wheat* clone() override;
};

#endif

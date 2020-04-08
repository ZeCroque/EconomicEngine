#ifndef LEATHER_H
#define LEATHER_H

#include "Countable.h"

class Leather final : public Countable
{
public:
	Leather();
	Leather(const Leather&) = default;
	Leather* clone() override;
};

#endif

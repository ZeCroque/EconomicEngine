#ifndef IRONORE_H
#define IRONORE_H

#include "Countable.h"

class IronOre final : public Countable
{
public:
	IronOre();
	IronOre(const IronOre&) = default;
	IronOre* clone() override;
};

#endif

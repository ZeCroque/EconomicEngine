#ifndef IRONORE_H
#define IRONORE_H

#include "Countable.h"

class IronOre final : public Countable
{
public:
	IronOre();
	IronOre(const IronOre& ironOre);
	IronOre* clone() override;
};

#endif

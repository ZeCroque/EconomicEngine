#ifndef WOODPICKAXE_H
#define WOODPICKAXE_H

#include "Uncountable.h"

class WoodPickaxe final : public Uncountable
{
public:
	WoodPickaxe();
	WoodPickaxe(const WoodPickaxe&) = default;
	WoodPickaxe* clone() override;
};

#endif

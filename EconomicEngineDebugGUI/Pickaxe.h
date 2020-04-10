#ifndef PICKAXE_H
#define PICKAXE_H

#include "Uncountable.h"

class Pickaxe final : public Uncountable
{
public:
	Pickaxe();
	Pickaxe(const Pickaxe&) = default;
	Pickaxe* clone() override;
};
#endif

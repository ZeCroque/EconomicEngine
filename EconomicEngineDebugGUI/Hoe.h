#ifndef HOE_H
#define HOE_H

#include "Uncountable.h"

class Hoe final : public Uncountable
{
public:
	Hoe();
	Hoe(const Hoe&) = default;
	Hoe* clone() override;
};
#endif

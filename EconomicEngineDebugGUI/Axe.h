#ifndef AXE_H
#define AXE_H

#include "Uncountable.h"

class Axe final : public Uncountable
{
public:
	Axe();
	Axe(const Axe&) = default;
	Axe* clone() override;
};
#endif

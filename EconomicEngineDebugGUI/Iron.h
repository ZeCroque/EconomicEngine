#ifndef IRON_H
#define IRON_H

#include "Countable.h"

class Iron final : public Countable
{
public:
	Iron();
	Iron(const Iron&) = default;
	Iron* clone() override;
};

#endif

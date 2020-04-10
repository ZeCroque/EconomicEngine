#ifndef BOW_H
#define BOW_H

#include "Uncountable.h"

class Bow final : public Uncountable
{
public:
	Bow();
	Bow(const Bow&) = default;
	Bow* clone() override;
};
#endif

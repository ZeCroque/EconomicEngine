#ifndef BREAD_H
#define BREAD_H

#include "Food.h"

class Bread final : public Countable
{
public:
	Bread();
	Bread(const Bread&) = default;
	Bread* clone() override;
};

#endif

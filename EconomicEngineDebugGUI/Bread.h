#ifndef BREAD_H
#define BREAD_H

#include "Food.h"

class Bread final : public Food
{
public:
	Bread();
	Bread(const Bread&) = default;
	Bread* clone() override;
};

#endif

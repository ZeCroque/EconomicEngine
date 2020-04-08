#ifndef WOOD_H
#define WOOD_H

#include "Countable.h"

class Wood final : public Countable
{
public:
	Wood();
	Wood(const Wood& wood);
	Wood* clone() override;
};

#endif

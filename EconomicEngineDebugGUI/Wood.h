#ifndef WOOD_H
#define WOOD_H

#include "Countable.h"

class Wood final : public Countable
{
public:
	Wood();
	Wood* clone() override;
};

#endif

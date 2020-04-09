#ifndef COAL_H
#define COAL_H

#include "Countable.h"

class Coal final : public Countable
{
public:
	Coal();
	Coal(const Coal&) = default;
	Coal* clone() override;
};

#endif

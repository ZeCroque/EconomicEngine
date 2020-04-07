#ifndef WHEAT_H
#define WHEAT_H

#include <vcruntime_typeinfo.h>

#include "Countable.h"

class Wheat : public Countable
{
public:
	Wheat::Wheat() : Countable()
	{
		this->name = "Wheat";
	}

	Wheat::Wheat(const Wheat& wheat) : Countable(wheat)
	{
		this->id = typeid(Wheat).hash_code();
	}

	Wheat* Wheat::clone() override
	{
		return new Wheat(*this);
	}
};

#endif

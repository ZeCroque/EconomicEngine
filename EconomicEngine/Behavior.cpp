#include "Behavior.h"

Behavior::Behavior() : item(nullptr), owner(nullptr), typeId(0){}

size_t Behavior::getId() const
{
	return typeId;
}

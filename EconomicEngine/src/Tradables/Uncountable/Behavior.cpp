#include "Tradables/Uncountable/Behavior.h"

Behavior::Behavior() : owningTool(nullptr), typeId(0){}

size_t Behavior::getId() const
{
	return typeId;
}

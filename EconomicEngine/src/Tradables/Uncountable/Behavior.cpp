#include "Tradables/Uncountable/Behavior.h"

Behavior::Behavior() : owningTool(nullptr), id(0)
{
}

Behavior::Behavior(const std::string& inName) : Behavior()
{
	const std::hash<std::string> hasher;
	id = hasher(inName);
}

size_t Behavior::getId() const
{
	return id;
}

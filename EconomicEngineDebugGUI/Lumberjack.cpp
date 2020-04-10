#include "Lumberjack.h"

#include "Wood.h"

Lumberjack::Lumberjack()
{
	//Wood : no requirement
	this->craftFactory->registerCraft(new Craft(0.5f, typeid(Wood).hash_code(), std::vector<std::pair<size_t, int>>()));
}

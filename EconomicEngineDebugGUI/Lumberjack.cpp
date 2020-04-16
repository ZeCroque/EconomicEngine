#include "Lumberjack.h"


#include "Axe.h"
#include "Wood.h"

Lumberjack::Lumberjack()
{
	this->id = typeid(Lumberjack).hash_code();
	//Wood : no requirement
	this->craftFactory->registerCraft(new Craft(0.5f, typeid(Wood).hash_code(), 1, std::vector<std::pair<size_t, int>>()));

	usableToolsList.emplace_back(typeid(Axe).hash_code());
}

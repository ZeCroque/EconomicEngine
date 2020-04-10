#include "Lumberjack.h"


#include "Axe.h"
#include "Wood.h"

Lumberjack::Lumberjack()
{
	//Wood : no requirement
	this->craftFactory->registerCraft(new Craft(0.5f, typeid(Wood).hash_code(), std::vector<std::pair<size_t, int>>()));

	std::vector<size_t> toolsRequired;
	toolsRequired.emplace_back(typeid(Axe).hash_code());
	this->craftFactory->registerCraft(new Craft(1.0f, typeid(Wood).hash_code(), std::vector<std::pair<size_t, int>>(),
	                                            toolsRequired));

	//usableToolsList.emplace_back(typeid(Axe).hash_code());
}

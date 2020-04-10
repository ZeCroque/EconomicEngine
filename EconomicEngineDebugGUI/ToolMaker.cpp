#include "ToolMaker.h"

#include "Hoe.h"
#include "Iron.h"
#include "Wood.h"
#include "WoodPickaxe.h"

ToolMaker::ToolMaker()
{
	std::vector<std::pair<size_t, int>> requirements;

	//WoodPickaxe : Need wood
	requirements.emplace_back(std::pair<size_t, int>(typeid(Wood).hash_code(), 1));
	this->craftFactory->registerCraft(new Craft(1.0f, typeid(WoodPickaxe).hash_code(), requirements));


	requirements.emplace_back(std::pair<size_t, int>(typeid(Iron).hash_code(), 1));
	this->craftFactory->registerCraft(new Craft(1.0f, typeid(Hoe).hash_code(), requirements));
}

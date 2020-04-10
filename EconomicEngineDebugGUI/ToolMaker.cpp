#include "ToolMaker.h"

#include "WoodPickaxe.h"
#include "Axe.h"
#include "Bow.h"
#include "Hoe.h"
#include "Pickaxe.h"
#include "Sword.h"

#include "Wood.h"
#include "Iron.h"
#include "Leather.h"

ToolMaker::ToolMaker()
{
	std::vector<std::pair<size_t, int>> requirements;

	//WoodPickaxe : Need wood
	requirements.emplace_back(std::pair<size_t, int>(typeid(Wood).hash_code(), 1));
	this->craftFactory->registerCraft(new Craft(1.0f, typeid(WoodPickaxe).hash_code(), requirements));


	requirements.emplace_back(std::pair<size_t, int>(typeid(Iron).hash_code(), 1));
	this->craftFactory->registerCraft(new Craft(1.0f, typeid(Hoe).hash_code(), requirements));
	this->craftFactory->registerCraft(new Craft(1.0f, typeid(Pickaxe).hash_code(), requirements));
	this->craftFactory->registerCraft(new Craft(1.0f, typeid(Axe).hash_code(), requirements));
	this->craftFactory->registerCraft(new Craft(1.0f, typeid(Bow).hash_code(), requirements));

	requirements.emplace_back(std::pair<size_t, int>(typeid(Leather).hash_code(), 1));
	this->craftFactory->registerCraft(new Craft(1.0f, typeid(Sword).hash_code(), requirements));
}

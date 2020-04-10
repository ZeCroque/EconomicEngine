#include "Miner.h"

#include "Coal.h"
#include "Iron.h"

#include "WoodPickaxe.h"

Miner::Miner()
{
	//Coal : no requirement
	this->craftFactory->registerCraft(new Craft(0.5f, typeid(Coal).hash_code(), std::vector<std::pair<size_t, int>>()));

	std::vector<size_t> toolsRequired;
	toolsRequired.emplace_back(typeid(WoodPickaxe).hash_code());

	this->craftFactory->registerCraft(new Craft(0.5f, typeid(Iron).hash_code(), std::vector<std::pair<size_t, int>>(), toolsRequired));

	usableToolsList.emplace_back(typeid(WoodPickaxe).hash_code());
}

#include "Miner.h"

#include "Coal.h"
#include "Iron.h"

#include "Pickaxe.h"
#include "WoodPickaxe.h"

Miner::Miner()
{
	this->id = typeid(Miner).hash_code();
	this->name = "Miner";
	
	//Coal : no requirement
	this->craftFactory->registerCraft(new Craft(0.5f, typeid(Coal).hash_code(), std::vector<std::pair<size_t, int>>()));

	std::vector<size_t> toolsRequired;

	toolsRequired.emplace_back(typeid(WoodPickaxe).hash_code());
	this->craftFactory->registerCraft(new Craft(0.5f, typeid(Iron).hash_code(), std::vector<std::pair<size_t, int>>(), toolsRequired));
	this->craftFactory->registerCraft(new Craft(1.0f, typeid(Coal).hash_code(), std::vector<std::pair<size_t, int>>(), toolsRequired));

	toolsRequired.clear();
	toolsRequired.emplace_back(typeid(Pickaxe).hash_code());
	this->craftFactory->registerCraft(new Craft(1.0f, typeid(Iron).hash_code(), std::vector<std::pair<size_t, int>>(),toolsRequired));

	usableToolsList.emplace_back(typeid(WoodPickaxe).hash_code());
	usableToolsList.emplace_back(typeid(Pickaxe).hash_code());
}

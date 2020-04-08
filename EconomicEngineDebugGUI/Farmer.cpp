#include "Farmer.h"
#include "Craft.h"

#include "Wheat.h"
#include "Bread.h"

Farmer::Farmer() : Job()
{
	//Wheat : no requirement
	this->craftFactory->
	      registerCraft(new Craft(0.5f, typeid(Wheat).hash_code(), std::vector<std::pair<size_t, int>>()));


	std::vector<std::pair<size_t, int>> requirements;

	//Bread : require one wheat
	requirements.emplace_back(std::pair<size_t, int>(typeid(Wheat).hash_code(), 1));
	this->craftFactory->registerCraft(new Craft(0.34f, typeid(Bread).hash_code(), requirements));

	//GoldenBread requires one bread one gold
	requirements.clear();
	requirements.emplace_back(std::pair<size_t, int>(typeid(Bread).hash_code(), 1));
	requirements.emplace_back(std::pair<size_t, int>(typeid(Gold).hash_code(), 1));
	this->craftFactory->registerCraft(new Craft(1.0f, typeid(GoldenBread).hash_code(), requirements));
}

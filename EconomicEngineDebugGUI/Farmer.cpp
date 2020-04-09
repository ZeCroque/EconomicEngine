#include "Farmer.h"

#include "Wheat.h"

Farmer::Farmer() : Job()
{
	//Wheat : no requirement
	this->craftFactory->
	      registerCraft(new Craft(1.0f, typeid(Wheat).hash_code(), std::vector<std::pair<size_t, int>>()));

	std::vector<size_t> toolsRequired;
	toolsRequired.emplace_back(typeid(Hoe).hash_code());
	
	this->craftFactory->registerCraft(new Craft(1.0f, typeid(GoldenBread).hash_code(), requirements, toolsRequired));

	usableToolsList.emplace_back(typeid(Hoe).hash_code());
}
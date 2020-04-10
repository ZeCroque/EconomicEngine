#include "Hunter.h"

#include "Bow.h"
#include "Sword.h"
#include "Leather.h"
#include "Meat.h"

Hunter::Hunter() : Job()
{
	this->id = typeid(Hunter).hash_code();
	//Steak : no requirement
	this->craftFactory->
	      registerCraft(new Craft(0.5f, typeid(Meat).hash_code(), std::vector<std::pair<size_t, int>>()));


	std::vector<size_t> toolsRequired;

	toolsRequired.emplace_back(typeid(Bow).hash_code());
	this->craftFactory->
	      registerCraft(new Craft(0.5f, typeid(Leather).hash_code(), std::vector<std::pair<size_t, int>>(),
	                              toolsRequired));
	toolsRequired.clear();
	toolsRequired.emplace_back(typeid(Sword).hash_code());
	this->craftFactory->
	      registerCraft(new Craft(1.0f, typeid(Meat).hash_code(), std::vector<std::pair<size_t, int>>(),
	                              toolsRequired));

	usableToolsList.emplace_back(typeid(Bow).hash_code());
	usableToolsList.emplace_back(typeid(Sword).hash_code());
}

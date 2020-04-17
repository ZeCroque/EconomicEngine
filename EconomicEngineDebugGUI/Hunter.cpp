#include "Hunter.h"

#include "Bow.h"
#include "BowBehavior.h"
#include "Sword.h"
#include "Leather.h"
#include "Meat.h"

#include <typeinfo>

Hunter::Hunter() : Job()
{
	this->id = typeid(Hunter).hash_code();
	this->name = "Hunter";
	
	//Steak : no requirement
	this->craftFactory->registerCraft(new Craft(0.5f, typeid(Meat).hash_code(), 5));


	std::list<size_t> toolsRequired;
	toolsRequired.emplace_back(typeid(BowBehavior).hash_code());
	this->craftFactory-> registerCraft(new Craft(0.5f, typeid(Leather).hash_code(), 2, std::list<std::pair<size_t, int>>(), toolsRequired));

	usableToolsList.emplace_back(typeid(Bow).hash_code());
	usableToolsList.emplace_back(typeid(Sword).hash_code());
}

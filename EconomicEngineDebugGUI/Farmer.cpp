#include "Farmer.h"


#include "Hoe.h"
#include "Uncountable.h"
#include "Wheat.h"

Farmer::Farmer() : Job()
{
	//Wheat : no requirement
	this->craftFactory->
	      registerCraft(new Craft(1.0f, typeid(Wheat).hash_code()));


	/*std::vector<size_t> toolsRequired;
	toolsRequired.emplace_back(typeid(Hoe).hash_code());

	this->craftFactory->registerCraft(new Craft(1.0f, typeid(Wheat).hash_code(), std::vector<std::pair<size_t, int>>(), toolsRequired));

	usableToolsList.emplace_back(typeid(Hoe).hash_code());*/
}

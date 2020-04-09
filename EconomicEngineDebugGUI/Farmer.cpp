#include "Farmer.h"

#include "Wheat.h"

Farmer::Farmer() : Job()
{
	//Wheat : no requirement
	this->craftFactory->
	      registerCraft(new Craft(1.0f, typeid(Wheat).hash_code(), std::vector<std::pair<size_t, int>>()));
}

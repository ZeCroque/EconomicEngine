#include "Farmer.h"



#include "Hoe.h"
#include "Uncountable.h"
#include "Wheat.h"

Farmer::Farmer() : Job()
{
	this->id = typeid(Farmer).hash_code();
	//Wheat : no requirement
	this->craftFactory-> registerCraft(new Craft(0.5f, typeid(Wheat).hash_code(), 3));


	usableToolsList.emplace_back(typeid(Hoe).hash_code());
}

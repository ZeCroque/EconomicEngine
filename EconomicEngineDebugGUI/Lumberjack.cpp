#include "Lumberjack.h"


#include "Axe.h"
#include "Wood.h"

#include <typeinfo>

Lumberjack::Lumberjack()
{
	this->id = typeid(Lumberjack).hash_code();
	this->name = "Lumberjack";
	
	//Wood : no requirement
	this->craftFactory->registerCraft(new Craft(0.5f, typeid(Wood).hash_code()));

	usableToolsList.emplace_back(typeid(Axe).hash_code());
}

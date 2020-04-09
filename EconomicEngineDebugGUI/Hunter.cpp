#include "Hunter.h"

#include "Meat.h"

Hunter::Hunter() : Job()
{
	//Steak : no requirement
	this->craftFactory->
	      registerCraft(new Craft(0.5f, typeid(Meat).hash_code(), std::vector<std::pair<size_t, int>>()));
}

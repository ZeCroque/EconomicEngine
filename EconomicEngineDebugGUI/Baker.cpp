#include "Baker.h"

#include "Wheat.h"
#include "Coal.h"

#include "Bread.h"

Baker::Baker() : Job()
{
	this->id = typeid(Baker).hash_code();
	this->name = "Baker";
	
	std::vector<std::pair<size_t, int>> requirements;

	//Bread : require wheat and coal
	requirements.emplace_back(std::pair<size_t, int>(typeid(Wheat).hash_code(), 3));
	requirements.emplace_back(std::pair<size_t, int>(typeid(Coal).hash_code(), 1));
	this->craftFactory->registerCraft(new Craft(1.0f, typeid(Bread).hash_code(), 10, requirements));
}

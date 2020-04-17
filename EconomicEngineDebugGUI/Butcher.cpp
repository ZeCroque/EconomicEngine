#include "Butcher.h"

#include "Meat.h"
#include "Coal.h"

#include "Steak.h"

Butcher::Butcher() : Job()
{
	this->id = typeid(Butcher).hash_code();
	this->name = "Butcher";
	
	std::vector<std::pair<size_t, int>> requirements;

	//Bread : require meat and coal
	requirements.emplace_back(std::pair<size_t, int>(typeid(Meat).hash_code(), 2));
	requirements.emplace_back(std::pair<size_t, int>(typeid(Coal).hash_code(), 4));
	this->craftFactory->registerCraft(new Craft(1.0f, typeid(Steak).hash_code(), 2, requirements));
}

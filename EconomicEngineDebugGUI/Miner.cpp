#include "Miner.h"

#include "Coal.h"

Miner::Miner()
{
	//Coal : no requirement
	this->craftFactory->registerCraft(new Craft(0.5f, typeid(Coal).hash_code(), std::vector<std::pair<size_t, int>>()));
}

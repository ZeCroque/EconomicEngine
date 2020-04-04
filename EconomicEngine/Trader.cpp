#include "Trader.h"
#include <chrono>
#include <random>

Trader::Trader()
{
	/*currentJob = nullptr;
	currentCraft = nullptr;*/
}

void Trader::makeAsks()
{
	std::mt19937 randomEngine(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
	const std::uniform_int_distribution<int> uniformDist(0, 1);
	for(auto tradable : wonderList)
	{	
		if(uniformDist(randomEngine))
		{
			//TODO
		}
	}

	for (auto tradable : goodsList)
	{
		if (uniformDist(randomEngine))
		{
			//TODO
		}
	}
}

void Trader::craft()
{
	
}

const std::list<std::shared_ptr<Tradable>>& Trader::getInventory() const
{
	return inventory;
}

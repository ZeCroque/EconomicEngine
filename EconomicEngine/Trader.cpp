#include "Trader.h"
#include <chrono>
#include <random>

Trader::Trader()
{
	currentJob = nullptr;
	currentCraft = nullptr;
}

void Trader::makeAsks()
{
	std::mt19937 randomEngine(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
	const std::uniform_int_distribution<int> uniformDist(0, 1);
	for(auto tradable : wonderList)
	{
		//TODO weight		
		if(uniformDist(randomEngine))
		{
			
			//BuyingAsk buyingAsk(tradable.get(), 10);
			//currentAsks.emplace_back(buyingAsk);
		}
	}

	for (auto tradable : goodsList)
	{
		//TODO weight
		if (uniformDist(randomEngine))
		{
			//SellingAsk buyingAsk(tradable.get(), 10);
			//currentAsks.emplace_back(buyingAsk);
		}
	}
}

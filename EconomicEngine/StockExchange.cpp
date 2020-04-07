#include "StockExchange.h"

StockExchange::StockExchange(const std::vector<size_t>& keys)
{
	this->keys = std::vector<size_t>(keys);
	this->currentBuyingAsks = VectorArray<BuyingAsk>(&this->keys);
	this->currentSellingAsks = VectorArray<SellingAsk>(&this->keys);
	this->betterAsks = VectorArray<SellingAsk>(&this->keys);
}

void StockExchange::registerAsk(std::shared_ptr<BuyingAsk>& buyingAsk)
{
	//auto& buyingAsks = currentBuyingAsks[buyingAsk->getId()];
	//buyingAsks.emplace_back(buyingAsk);
	//insertionSort(buyingAsks);
}

void StockExchange::registerAsk(const std::shared_ptr<SellingAsk>& sellingAsk)
{
	//auto& sellingAsks = currentBuyingAsks[sellingAsk->getId()];
	//sellingAsks.emplace_back(sellingAsk);
	//insertionSort(sellingAsks);
}



void StockExchange::resolveOffers()
{
	for(auto key : this->keys)
	{
		auto& buyingAsks = currentBuyingAsks[key];
		auto& sellingAsks = currentBuyingAsks[key];
		while (!buyingAsks.empty() && !sellingAsks.empty())
		{
			for (int i = 0; i < static_cast<int>(sellingAsks.size()); ++i)
			{
				for (int j = buyingAsks.size(); j > 0; --j)
				{
					//On teste si les offres ont la même Commodity et le même prix
					if (buyingAsks[j]->getPrice() == sellingAsks[i]->getPrice())
					{
						//Puis on enregistre la transaction avant de supprimer les offres
						//betterAsks[key].emplace_back(sellingAsks[i]);
						
						sellingAsks.erase(sellingAsks.begin() + j);
						buyingAsks.erase(buyingAsks.begin() + i);
					}
				}
			}
		}
	}
}


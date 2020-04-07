#include "StockExchange.h"

#include <memory>

void StockExchange::setKeys(const std::vector<size_t>& keys)
{
	this->keys = std::vector<size_t>(keys);
	this->currentBuyingAsks = VectorArray<BuyingAsk>(this->keys);
	this->currentSellingAsks = VectorArray<SellingAsk>(this->keys);
	this->betterAsks = VectorArray<SellingAsk>(this->keys);
}

void StockExchange::registerAsk(std::shared_ptr<BuyingAsk> buyingAsk)
{
	auto& buyingAsks = currentBuyingAsks[buyingAsk->getId()];
	buyingAsks.emplace_back(std::move(buyingAsk));
	insertionSort(buyingAsks);
}

void StockExchange::registerAsk(std::shared_ptr<SellingAsk> sellingAsk)
{
	auto& sellingAsks = currentSellingAsks[sellingAsk->getId()];
	sellingAsks.emplace_back(std::move(sellingAsk));
	insertionSort(sellingAsks);
}

void StockExchange::resolveOffers()
{
	for(auto key : this->keys)
	{
		auto& buyingAsks = currentBuyingAsks[key];
		auto& sellingAsks = currentSellingAsks[key];
		while (!buyingAsks.empty() && !sellingAsks.empty())
		{
			for (size_t i = 0; i < sellingAsks.size(); ++i)
			{
				for (auto j = buyingAsks.size(); j > 0; --j)
				{
					//On teste si les offres ont la même Commodity et le même prix
					if (buyingAsks[j]->getPrice() == sellingAsks[i]->getPrice())
					{
						//Puis on enregistre la transaction avant de supprimer les offres
						betterAsks[key].emplace_back(sellingAsks[i]);

						sellingAsks[i].reset();
						sellingAsks.erase(sellingAsks.begin() + i);
						buyingAsks[i].reset();
						buyingAsks.erase(buyingAsks.begin() + j);
					}
				}
			}
		}
	}
}


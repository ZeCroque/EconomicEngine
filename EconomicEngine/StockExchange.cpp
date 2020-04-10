#include "StockExchange.h"

#include <memory>

void StockExchange::setKeys(const std::vector<size_t>& keys)
{
	this->keys = std::vector<size_t>(keys);
	this->currentBuyingAsks = VectorArray<BuyingAsk>(this->keys);
	this->currentSellingAsks = VectorArray<SellingAsk>(this->keys);
	this->betterAsks = VectorArray<BuyingAsk>(this->keys);
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
	for (auto key : this->keys)
	{
		auto& buyingAsks = currentBuyingAsks[key];
		auto& sellingAsks = currentSellingAsks[key];
		bool doOnce = true;
		while (!buyingAsks.empty() && !sellingAsks.empty() && buyingAsks[buyingAsks.size() - 1]->getPrice() > sellingAsks[0]->getPrice()) //TODO revert sort selling to avoid reallocating
		{
			if (doOnce)
			{
				doOnce = false;
				betterAsks[key].emplace_back(buyingAsks[buyingAsks.size() - 1]);
			}
			sellingAsks[0]->setPrice(buyingAsks[buyingAsks.size() - 1]->getPrice());
			sellingAsks[0]->setStatus(AskStatus::Sold);
			sellingAsks[0].reset();
			sellingAsks.erase(sellingAsks.begin());
			buyingAsks[buyingAsks.size() - 1]->setStatus(AskStatus::Sold);
			buyingAsks[buyingAsks.size() - 1].reset();
			buyingAsks.erase(buyingAsks.begin() + buyingAsks.size() - 1);
			
		}
		
		for (auto& sellingAsk : sellingAsks)
		{
			sellingAsk->setStatus(AskStatus::Refused);
			sellingAsk.reset();
		}

		for(auto& buyingAsk : buyingAsks)
		{
			buyingAsk->setStatus(AskStatus::Refused);
			buyingAsk.reset();
		}

		buyingAsks.clear();
		sellingAsks.clear();
	}

}

void StockExchange::reset()
{
	for(auto key : keys)
	{
		currentSellingAsks[key].clear();
		currentBuyingAsks[key].clear();
		betterAsks[key].clear();
	}

}

float StockExchange::getStockExchangePrice(const size_t key)
{
	float result = 0.0f;
	if(!betterAsks[key].empty())
	{
		result = betterAsks[key].back()->getPrice();
	}
	return result;
}

std::list<BuyingAsk> StockExchange::getStockExchangePrice(size_t key, int count)
{
	std::list<BuyingAsk> result;
	int i = 0;
	for(auto it = betterAsks[key].rbegin(); it!= betterAsks[key].rend() && i < count; ++it, ++i)
	{
		result.emplace_front(*it->get());

	}
	return result;
}


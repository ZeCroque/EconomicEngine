#include "StockExchange.h"

StockExchange::StockExchange(const std::vector<size_t>& keys)
{
	this->keys = std::vector<size_t>(keys);
	this->currentBuyingAsks = VectorArray<BuyingAsk>(&this->keys);
	this->currentSellingAsks = VectorArray<SellingAsk>(&this->keys);
	this->betterAsks = VectorArray<BuyingAsk>(&this->keys);
}

void StockExchange::registerBuyingAsk(const std::shared_ptr<BuyingAsk>& buyingAsk)
{
	currentBuyingAsks[buyingAsk->getId()].emplace_back(buyingAsk);
}

void StockExchange::registerSellingAsk(const std::shared_ptr<SellingAsk>& sellingAsk)
{
	currentSellingAsks[sellingAsk->getId()].emplace_back(sellingAsk);
}

void StockExchange::resolveOffers()
{
	for(auto key : this->keys)
	{
		for(auto ask : (this->currentBuyingAsks[key]))
		{
			//TODO Maxence
		}
	}
	
}


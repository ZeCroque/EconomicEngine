#include "StockExchange.h"
#include <typeinfo>
#include "Money.h"

void StockExchange::registerBuyingAsk(BuyingAsk&  buyingAsk)
{
	currentBuyingAsks[typeid(buyingAsk.getContent()[0].get()).hash_code()]->emplace_back(buyingAsk);
}

void StockExchange::registerSellingAsk(SellingAsk& sellingAsk)
{
	currentSellingAsks[typeid(sellingAsk.getContent()[0].get()).hash_code()]->emplace_back(sellingAsk);
}

void StockExchange::resolveOffers()
{
	for(auto keys : this->keys)
	{
		for(auto& ask : (*this->currentBuyingAsks[typeid(Money).hash_code()]))
		{
			
		}
	}
	
}


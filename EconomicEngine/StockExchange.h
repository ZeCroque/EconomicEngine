#ifndef STOCK_EXCHANGE_H
#define STOCK_EXCHANGE_H
#include <map>
#include <typeinfo>
#include "Ask.h"
#include "Tradable.h"

class StockExchange
{
private:
	std::multimap<std::type_info, std::reference_wrapper<BuyingAsk>> currentBuyingAsks;
	std::multimap<std::type_info, std::reference_wrapper<SellingAsk>> currentSellingAsks;
	std::multimap<std::type_info, std::reference_wrapper<BuyingAsk>> betterAsks;
	
public:
	StockExchange() = default;
	
	void registerBuyingAsk(BuyingAsk<Tradable>& buyingAsk);	
	{
		currentSellingAsks.emplace(typeid(buyingAsk), buyingAsk);
	}
	
	void registerSellingAsk(SellingAsk<Tradable>& sellingAsk)
	{
		currentSellingAsks.emplace(typeid(sellingAsk), sellingAsk);
	}

	void resolveOffers()
	{
		//TODO Maxence
	}
};

#endif

#ifndef STOCK_EXCHANGE_H
#define STOCK_EXCHANGE_H
#include <map>
#include <typeinfo>
#include "Ask.h"
#include "Currency.h"

template <class T> class StockExchange
{
private:
	T* currency;
	std::multimap<std::type_info, std::reference_wrapper<BuyingAsk<Tradable>>> currentBuyingAsks;
	std::multimap<std::type_info, std::reference_wrapper<SellingAsk<Tradable>>> currentSellingAsks;
	std::multimap<std::type_info, std::reference_wrapper<BuyingAsk<Tradable>>> betterAsks;
	
public:
	StockExchange()
	{
		static_assert(std::is_base_of<Currency, T>::value);
		currency = T::getInstance();
	}
	
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

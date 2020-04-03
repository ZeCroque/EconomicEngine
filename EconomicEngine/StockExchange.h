#ifndef STOCK_EXCHANGE_H
#define STOCK_EXCHANGE_H
#include <list>
#include "Ask.h"
#include "Tradable.h"

class StockExchange
{
private:
	std::list<std::reference_wrapper<BuyingAsk>> currentBuyingAsks;
	std::list<std::reference_wrapper<SellingAsk>> currentSellingAsks;
	std::vector<std::reference_wrapper<BuyingAsk>>* betterAsks;
	
public:
	StockExchange();
	~StockExchange();
	
	void registerBuyingAsk(const BuyingAsk& buyingAsk);	
	void registerSellingAsk(const SellingAsk& sellingAsk);

	void resolveOffers(); //TODO Maxence

};



#endif

#ifndef STOCK_EXCHANGE_H
#define STOCK_EXCHANGE_H
#include "Ask.h"
#include "VectorArray.h"

class StockExchange
{
private:
	VectorArray<BuyingAsk> currentBuyingAsks;
	VectorArray<SellingAsk> currentSellingAsks;
	VectorArray<BuyingAsk> betterAsks;
	std::vector<size_t> keys;
	
public:
	StockExchange() = default;
	StockExchange(const std::vector<size_t>& keys);

	void registerBuyingAsk(const std::shared_ptr<BuyingAsk>& buyingAsk);	
	void registerSellingAsk(const std::shared_ptr<SellingAsk>& sellingAsk);

	void resolveOffers(); //TODO Maxence

};



#endif

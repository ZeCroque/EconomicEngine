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
	StockExchange(const std::vector<size_t>& keys)
	{
		this->keys = std::vector<size_t>(keys);
		this->currentBuyingAsks = VectorArray<BuyingAsk>(&this->keys);
		this->currentSellingAsks = VectorArray<SellingAsk>(&this->keys);
		this->betterAsks = VectorArray<BuyingAsk>(&this->keys);
		
	}

	void registerBuyingAsk(BuyingAsk& buyingAsk);	
	void registerSellingAsk(SellingAsk& sellingAsk);

	void resolveOffers(); //TODO Maxence

};



#endif

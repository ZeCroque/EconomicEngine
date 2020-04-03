#include "StockExchange.h"

StockExchange::StockExchange()
{
	betterAsks = new std::vector<std::reference_wrapper<BuyingAsk>>[10]; //TODO variable
}

StockExchange::~StockExchange()
{
	delete betterAsks;
}


void StockExchange::registerBuyingAsk(const BuyingAsk&  buyingAsk)
{
	currentBuyingAsks.emplace_back(buyingAsk);
}

void StockExchange::registerSellingAsk(const SellingAsk& sellingAsk)
{
	currentSellingAsks.emplace_back(sellingAsk);
}


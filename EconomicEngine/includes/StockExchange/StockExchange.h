#ifndef STOCK_EXCHANGE_H
#define STOCK_EXCHANGE_H

#include <list>

#include "Ask.h"
#include "VectorArray.h"

class StockExchange
{
public:
	void init();
	void registerAsk(std::shared_ptr<BuyingAsk> buyingAsk);	
	void registerAsk(std::shared_ptr<SellingAsk> sellingAsk);
	void resolveOffers();
	void reset();
	[[nodiscard]] float getStockExchangePrice(size_t key) const;
	[[nodiscard]] std::list<BuyingAsk> getStockExchangePrice(size_t key, int count) const;
	[[nodiscard]] const Signal<>& getAskResolvedSignal() const;
	
	template <class T> static void insertionSort(std::vector<std::shared_ptr<T>>& vector)
	{
		for (size_t i = 1; i < vector.size(); ++i)
		{
			auto temp = vector[i];
			vector[i].reset();
			int j = static_cast<int>(i)-1;
			while (j >= 0 && vector[j]->getPrice() > temp->getPrice())
			{
				auto temp2 = vector[j];
				vector[j].reset();
				vector[j + 1] = temp2;
				--j;
			}
			vector[j + 1] = temp;
		}
	}

private:
	VectorArray<BuyingAsk> currentBuyingAsks;
	VectorArray<SellingAsk> currentSellingAsks;
	VectorArray<BuyingAsk> betterAsks;
	Signal<> askResolvedSignal;

};



#endif

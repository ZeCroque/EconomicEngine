#ifndef STOCK_EXCHANGE_H
#define STOCK_EXCHANGE_H

#include <list>

#include "Ask.h"
#include "VectorArray.h"

class StockExchange
{
public:
	void init();
	void registerAsk(std::shared_ptr<Ask> inAsk);
	void removeAsk(std::shared_ptr<Ask> inAsk);
	void resolveOffers();
	void reset();
	[[nodiscard]] float getStockExchangePrice(size_t inKey) const;
	[[nodiscard]] std::list<Ask> getStockExchangePrice(size_t inKey, int inCount) const;
	[[nodiscard]] const Signal<>& getAskResolvedSignal() const;
	
	template <class T> static void insertionSort(std::vector<std::shared_ptr<T>>& outVector)
	{
		for (size_t i = 1; i < outVector.size(); ++i)
		{
			auto temp = outVector[i];
			outVector[i].reset();
			int j = static_cast<int>(i)-1;
			while (j >= 0 && outVector[j]->getPrice() > temp->getPrice())
			{
				auto temp2 = outVector[j];
				outVector[j].reset();
				outVector[j + 1] = temp2;
				--j;
			}
			outVector[j + 1] = temp;
		}
	}

private:
	VectorArray<Ask> currentBuyingAsks;
	VectorArray<Ask> currentSellingAsks;
	VectorArray<Ask> betterAsks;
	Signal<> askResolvedSignal;

};



#endif //STOCK_EXCHANGE_H

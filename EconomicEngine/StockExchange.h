#ifndef STOCK_EXCHANGE_H
#define STOCK_EXCHANGE_H
#include <algorithm>

#include "Ask.h"
#include "Singleton.h"
#include "VectorArray.h"

class StockExchange : public Singleton<StockExchange>
{
	friend class TurnManager; //DEBUG
	
private:
	VectorArray<BuyingAsk> currentBuyingAsks;
	VectorArray<SellingAsk> currentSellingAsks;
	VectorArray<SellingAsk> betterAsks;
	std::vector<size_t> keys;
	
public:
	void setKeys(const std::vector<size_t>& keys);

	void registerAsk(std::shared_ptr<BuyingAsk> buyingAsk);	
	void registerAsk(std::shared_ptr<SellingAsk> sellingAsk);
	void resolveOffers(); //TODO Maxence
	float getStockExchangePrice(size_t key);
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



};

#endif

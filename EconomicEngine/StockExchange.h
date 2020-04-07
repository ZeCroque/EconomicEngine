#ifndef STOCK_EXCHANGE_H
#define STOCK_EXCHANGE_H
#include "Ask.h"
#include "VectorArray.h"

class StockExchange
{
private:
	VectorArray<BuyingAsk> currentBuyingAsks;
	VectorArray<SellingAsk> currentSellingAsks;
	VectorArray<SellingAsk> betterAsks;
	std::vector<size_t> keys;
	
public:
	StockExchange() = default;
	StockExchange(const std::vector<size_t>& keys);

	void registerAsk(std::shared_ptr<BuyingAsk>& buyingAsk);	
	void registerAsk(const std::shared_ptr<SellingAsk>& sellingAsk);
	void resolveOffers(); //TODO Maxence
	/*template <class T> static void insertionSort(std::vector<std::shared_ptr<T>>& vector)
	{
		for (int i = 1; i < vector.size(); i++)
		{
			auto& temp = vector[i];
			int j = i - 1;
			while (j >= 0 && temp->getPrice() <= vector[j]->getPrice())
			{
				vector[j + 1] = vector[j];
				j = j - 1;
			}
			vector[j + 1] = temp;
		}
	}*/



};

#endif

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
	template <class T> static void insertionSort(std::vector<std::shared_ptr<T>>& vector)
	{
		for (size_t i = 1; i < vector.size(); i++)
		{
			auto& temp = vector[i];
			size_t j = i;
			while (j > 0 && vector[j - 1]->getPrice() > temp->getPrice() )
			{
				auto& temp2 = vector[j-1];
				vector.erase(vector.begin()+j);
				vector.emplace(vector.begin() + std::max<int>(0, j - 2), temp2);
				//vector[j+1] = vector[j];
				--j;
			}
			vector.erase(vector.begin() + j);
			vector.emplace(vector.begin() + std::max<int>(0,j - 2), temp);
		}
		int i = 42;
	}



};

#endif

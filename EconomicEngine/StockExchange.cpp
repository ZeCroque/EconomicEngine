#include "StockExchange.h"

#include <algorithm>
#include <memory>

#include "TradableManager.h"

StockExchange::StockExchange() : turnCount(0) {}

void StockExchange::init()
{
	this->keys = std::vector<size_t>(TradableManager::getInstance()->getKeys());
	this->currentBuyingAsks = VectorArray<BuyingAsk>(this->keys);
	this->currentSellingAsks = VectorArray<SellingAsk>(this->keys);
	this->betterAsks = VectorArray<BuyingAsk>(this->keys);
	for (auto key : keys)
	{
		betterAsks[key].emplace_back(std::make_shared<BuyingAsk>(BuyingAsk(key, 0, 0.0f)));
	}
}

void StockExchange::registerAsk(std::shared_ptr<BuyingAsk> buyingAsk)
{
	auto& buyingAsks = currentBuyingAsks[buyingAsk->getId()];
	buyingAsks.emplace_back(std::move(buyingAsk));
	insertionSort(buyingAsks);
}

void StockExchange::registerAsk(std::shared_ptr<SellingAsk> sellingAsk)
{
	auto& sellingAsks = currentSellingAsks[sellingAsk->getId()];
	sellingAsks.emplace_back(std::move(sellingAsk));
	insertionSort(sellingAsks);
}

void StockExchange::resolveOffers()
{
	for (auto key : this->keys)
	{
		auto& buyingAsks = currentBuyingAsks[key];
		auto& sellingAsks = currentSellingAsks[key];
		bool doOnce = true;
		while (!buyingAsks.empty() && !sellingAsks.empty() && buyingAsks[buyingAsks.size() - 1]->getPrice() > sellingAsks[0]->getPrice() && buyingAsks[buyingAsks.size() - 1]->getCount() < sellingAsks[0]->getCount())
		{
			if (doOnce)
			{
				doOnce = false;
				betterAsks[key].emplace_back(buyingAsks[buyingAsks.size() - 1]);
			}
			
			const int tradedCount = std::min<int>(buyingAsks[buyingAsks.size() - 1]->getCount() - buyingAsks[buyingAsks.size() - 1]->getTradedCount(), sellingAsks[0]->getCount() - sellingAsks[0]->getTradedCount());
			
			sellingAsks[0]->setPrice(buyingAsks[buyingAsks.size() - 1]->getPrice());
			sellingAsks[0]->incrementTradedCountBy(tradedCount);
			sellingAsks[0]->setStatus(AskStatus::Sold);
			if (sellingAsks[0]->getCount() == sellingAsks[0]->getTradedCount())
			{
				sellingAsks.erase(sellingAsks.begin());
			}

			buyingAsks[buyingAsks.size() - 1]->incrementTradedCountBy(tradedCount);
			buyingAsks[buyingAsks.size() - 1]->setStatus(AskStatus::Sold);
			if(buyingAsks[buyingAsks.size() - 1]->getCount() == buyingAsks[buyingAsks.size() - 1]->getTradedCount())
			{
				buyingAsks.erase(buyingAsks.begin() + buyingAsks.size() - 1);
			}
		}
		if (doOnce)
		{
			betterAsks[key].emplace_back(betterAsks[key][betterAsks[key].size() - 1]);
		}

		for (auto& sellingAsk : sellingAsks)
		{
			if(sellingAsk->getStatus()==AskStatus::Pending)
			{
				sellingAsk->setStatus(AskStatus::Refused);
			}
		}

		for (auto& buyingAsk : buyingAsks)
		{
			if (buyingAsk->getStatus() == AskStatus::Pending)
			{
				buyingAsk->setStatus(AskStatus::Refused);
			}
		}

		buyingAsks.clear();
		sellingAsks.clear();
	}
}

void StockExchange::reset()
{
	for (auto key : keys)
	{
		currentSellingAsks[key].clear();
		currentBuyingAsks[key].clear();
		betterAsks[key].clear();
	}

	for (auto key : keys)
	{
		betterAsks[key].emplace_back(std::make_shared<BuyingAsk>(BuyingAsk(key, 0, 0.0f)));
	}

	turnCount = 0;
}

void StockExchange::incrementTurnCount()
{
	++this->turnCount;
}

float StockExchange::getStockExchangePrice(const size_t key)
{
	float result = 0.0f;
	if (!betterAsks[key].empty())
	{
		result = betterAsks[key].back()->getPrice();
	}
	return result;
}

std::list<BuyingAsk> StockExchange::getStockExchangePrice(size_t key, int count)
{
	std::list<BuyingAsk> result;
	int i = 0;
	for (auto it = betterAsks[key].rbegin(); it != betterAsks[key].rend() && i < count; ++it, ++i)
	{
		result.emplace_front(*it->get());
	}
	return result;
}

int StockExchange::getTurnCount() const
{
	return turnCount;
}


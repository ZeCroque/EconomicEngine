#include "StockExchange/StockExchange.h"
#include <algorithm>
#include <iostream>


#include "EconomicEngine.h"
#include "Traders/Trader.h"

void StockExchange::init()
{
	auto keys = EconomicEngine::getInstance()->getTradableFactory().getKeys();
	currentBuyingAsks = VectorArray<Ask>(keys);
	currentSellingAsks = VectorArray<Ask>(keys);
	betterAsks = VectorArray<Ask>(keys);
	for (auto key : keys)
	{
		betterAsks[key].emplace_back(std::make_shared<Ask>(false,key, 0, 0.0f));
	}
}

void StockExchange::registerAsk(std::shared_ptr<Ask> ask)
{
	if(ask->getIsSellingAsk())
	{
		auto& sellingAsks = currentSellingAsks[ask->getId()];
		sellingAsks.emplace_back(std::move(ask));
		insertionSort(sellingAsks);
	}
	else
	{
		auto& buyingAsks = currentBuyingAsks[ask->getId()];
		buyingAsks.emplace_back(std::move(ask));
		insertionSort(buyingAsks);
	}
}

void StockExchange::resolveOffers()
{
	for (auto key : EconomicEngine::getInstance()->getTradableFactory().getKeys())
	{
		auto& buyingAsks = currentBuyingAsks[key];
		auto& sellingAsks = currentSellingAsks[key];

		bool askResolved = false;
		while (!buyingAsks.empty() && !sellingAsks.empty() && buyingAsks.back()->getPrice() > sellingAsks.front()->getPrice() && buyingAsks.back()->getCount() < sellingAsks.front()->getCount())
		{			
			const int tradedCount = std::min<int>(buyingAsks.back()->getCount() - buyingAsks.back()->getTradedCount(), sellingAsks.front()->getCount() - sellingAsks.front()->getTradedCount());
			const float price = (buyingAsks.back()->getPrice() + sellingAsks.front()->getPrice()) / 2;

			sellingAsks.front()->setPrice(price);
			sellingAsks.front()->incrementTradedCountBy(tradedCount);
			sellingAsks.front()->setStatus(AskStatus::Sold);
			if (sellingAsks.front()->getCount() == sellingAsks.front()->getTradedCount())
			{
				sellingAsks.front()->resolve();
				sellingAsks.erase(sellingAsks.begin());
			}
			
			buyingAsks.back()->setPrice(price);
			buyingAsks.back()->incrementTradedCountBy(tradedCount);
			buyingAsks.back()->setStatus(AskStatus::Sold);

			if (!askResolved)
			{
				askResolved = true;
				betterAsks[key].emplace_back(buyingAsks.back());
			}
			
			if(buyingAsks.back()->getCount() == buyingAsks.back()->getTradedCount())
			{
				buyingAsks.back()->resolve();
				buyingAsks.pop_back();
			}
		}
		if (!askResolved)
		{
			betterAsks[key].emplace_back(betterAsks[key].back());
		}

		for (auto& sellingAsk : sellingAsks)
		{
			if(sellingAsk->getStatus()==AskStatus::Pending)
			{
				sellingAsk->setStatus(AskStatus::Refused);
			}
			sellingAsk->resolve();
		}

		for (auto& buyingAsk : buyingAsks)
		{
			if (buyingAsk->getStatus() == AskStatus::Pending)
			{
				buyingAsk->setStatus(AskStatus::Refused);
			}
			buyingAsk->resolve();
		}

		buyingAsks.clear();
		sellingAsks.clear();
	}
	askResolvedSignal();
}

void StockExchange::reset()
{
	for (auto key : EconomicEngine::getInstance()->getTradableFactory().getKeys())
	{
		currentSellingAsks[key].clear();
		currentBuyingAsks[key].clear();
		betterAsks[key].clear();
		betterAsks[key].emplace_back(std::make_shared<Ask>(false,key, 0, 0.0f));
	}
}

float StockExchange::getStockExchangePrice(const size_t key) const
{
	float result = 0.0f;
	if (!betterAsks[key].empty())
	{
		result = betterAsks[key].back()->getPrice();
	}
	return result;
}

std::list<Ask> StockExchange::getStockExchangePrice(const size_t key, const int count) const
{
	std::list<Ask> result;
	int i = 0;
	for (auto it = betterAsks[key].rbegin(); it != betterAsks[key].rend() && i < count; ++it, ++i)
	{
		result.emplace_front(*it->get());
	}
	return result;
}

const Signal<>& StockExchange::getAskResolvedSignal() const
{
	return askResolvedSignal;
}


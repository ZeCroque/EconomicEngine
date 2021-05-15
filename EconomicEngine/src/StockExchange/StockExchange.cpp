#include <algorithm>
#include <iostream>

#include "StockExchange/StockExchange.h"
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
		betterAsks[key].push_back(std::make_shared<Ask>(false,key, 0, 0.0f));
	}
}

void StockExchange::registerAsk(std::shared_ptr<Ask> inAsk)
{
	if(inAsk->isSellingAsk())
	{
		auto& sellingAsks = currentSellingAsks[inAsk->getId()];
		sellingAsks.push_back(std::move(inAsk));
		insertionSort(sellingAsks);
	}
	else
	{
		auto& buyingAsks = currentBuyingAsks[inAsk->getId()];
		buyingAsks.push_back(std::move(inAsk));
		insertionSort(buyingAsks);
	}
}

void StockExchange::removeAsk(const std::shared_ptr<Ask> inAsk)  // NOLINT(performance-unnecessary-value-param)
{
	if(auto & registeredAsks = inAsk->isSellingAsk() ? currentSellingAsks[inAsk->getId()] : currentBuyingAsks[inAsk->getId()]; inAsk && !registeredAsks.empty())
	{
		int i = 0;
		for(const auto& registeredAsk : registeredAsks)
		{
			if(registeredAsk.get() == inAsk.get())
			{
				break;
			}
			++i;
		}
		registeredAsks.erase(registeredAsks.begin() + i);
	}
}

void StockExchange::resolveOffers()
{
	for (auto key : EconomicEngine::getInstance()->getTradableFactory().getKeys())
	{
		auto& buyingAsks = currentBuyingAsks[key];
		auto& sellingAsks = currentSellingAsks[key];

		bool bAskResolved = false;
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

			if (!bAskResolved)
			{
				bAskResolved = true;
				betterAsks[key].push_back(buyingAsks.back());
			}
			
			if(buyingAsks.back()->getCount() == buyingAsks.back()->getTradedCount())
			{
				buyingAsks.back()->resolve();
				buyingAsks.pop_back();
			}
		}
		if (!bAskResolved)
		{
			betterAsks[key].push_back(betterAsks[key].back());
		}

		for (auto& sellingAsk : sellingAsks)
		{
			if(sellingAsk->getStatus() == AskStatus::Pending)
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
		betterAsks[key].push_back(std::make_shared<Ask>(false,key, 0, 0.0f));
	}
}

float StockExchange::getStockExchangePrice(const size_t inKey) const
{
	float result = 0.0f;
	if (!betterAsks[inKey].empty())
	{
		result = betterAsks[inKey].back()->getPrice();
	}
	return result;
}

std::list<Ask> StockExchange::getStockExchangePrice(const size_t inKey, const int inCount) const
{
	std::list<Ask> result;
	int i = 0;
	for (auto it = betterAsks[inKey].rbegin(); it != betterAsks[inKey].rend() && i < inCount; ++it, ++i)
	{
		result.emplace_front(*it->get());
	}
	return result;
}

const Signal<>& StockExchange::getAskResolvedSignal() const
{
	return askResolvedSignal;
}


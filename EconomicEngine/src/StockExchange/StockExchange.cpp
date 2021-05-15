#include <algorithm>
#include <iostream>

#include "StockExchange/StockExchange.h"
#include "EconomicEngine.h"
#include "Traders/Trader.h"

void StockExchange::init()
{
	auto itemsIds = EconomicEngine::getInstance()->getTradableFactory().getKeys();
	currentBuyingAsks = VectorArray<Ask>(itemsIds);
	currentSellingAsks = VectorArray<Ask>(itemsIds);
	betterAsks = VectorArray<Ask>(itemsIds);
	for (auto itemId : itemsIds)
	{
		betterAsks[itemId].push_back(std::make_shared<Ask>(false,itemId, 0, 0.0f));
	}
}

void StockExchange::registerAsk(std::shared_ptr<Ask> inAsk)
{
	if(inAsk->isSellingAsk())
	{
		auto& sellingAsks = currentSellingAsks[inAsk->getItemId()];
		sellingAsks.push_back(std::move(inAsk));
		insertionSort(sellingAsks);
	}
	else
	{
		auto& buyingAsks = currentBuyingAsks[inAsk->getItemId()];
		buyingAsks.push_back(std::move(inAsk));
		insertionSort(buyingAsks);
	}
}

void StockExchange::removeAsk(const std::shared_ptr<Ask> inAsk)  // NOLINT(performance-unnecessary-value-param)
{
	if(auto & registeredAsks = inAsk->isSellingAsk() ? currentSellingAsks[inAsk->getItemId()] : currentBuyingAsks[inAsk->getItemId()]; inAsk && !registeredAsks.empty())
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
	for (auto itemId : EconomicEngine::getInstance()->getTradableFactory().getKeys())
	{
		auto& buyingAsks = currentBuyingAsks[itemId];
		auto& sellingAsks = currentSellingAsks[itemId];

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
				betterAsks[itemId].push_back(buyingAsks.back());
			}
			
			if(buyingAsks.back()->getCount() == buyingAsks.back()->getTradedCount())
			{
				buyingAsks.back()->resolve();
				buyingAsks.pop_back();
			}
		}
		if (!bAskResolved)
		{
			betterAsks[itemId].push_back(betterAsks[itemId].back());
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
	for (auto itemId : EconomicEngine::getInstance()->getTradableFactory().getKeys())
	{
		currentSellingAsks[itemId].clear();
		currentBuyingAsks[itemId].clear();
		betterAsks[itemId].clear();
		betterAsks[itemId].push_back(std::make_shared<Ask>(false,itemId, 0, 0.0f));
	}
}

float StockExchange::getCurrentMarketPrice(const size_t inItemId) const
{
	float result = 0.0f;
	if (!betterAsks[inItemId].empty())
	{
		result = betterAsks[inItemId].back()->getPrice();
	}
	return result;
}

std::list<Ask> StockExchange::getHistoricMarketPrices(const size_t inItemId, const int inDeepness) const
{
	std::list<Ask> result;
	int i = 0;
	for (auto it = betterAsks[inItemId].rbegin(); it != betterAsks[inItemId].rend() && i < inDeepness; ++it, ++i)
	{
		result.emplace_front(*it->get());
	}
	return result;
}

const Signal<>& StockExchange::getAskResolvedSignal() const
{
	return askResolvedSignal;
}


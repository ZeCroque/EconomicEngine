#include "StockExchange/Ask.h"

Ask::Ask(const bool bInIsSellingAsk, const size_t inItemId, const int inCount, const float inPrice) : count(inCount),
	price(inPrice), bIsSellingAsk(bInIsSellingAsk), status(AskStatus::Pending), tradableId(inItemId), tradedCount(0)
{
}

void Ask::resolve()
{
	askResolvedSignal(this);
}

bool Ask::isSellingAsk() const
{
	return bIsSellingAsk;
}

float Ask::getPrice() const
{
	return price;
}

int Ask::getCount() const
{
	return count;
}

size_t Ask::getItemId() const
{
	return tradableId;
}

AskStatus Ask::getStatus() const
{
	return status;
}

void Ask::setPrice(const float inPrice)
{
	price = inPrice;
}

void Ask::setStatus(const AskStatus inStatus)
{
	status = inStatus;
}

int Ask::getTradedCount() const
{
	return tradedCount;
}

const Signal<Ask*>& Ask::getAskResolvedSignal() const
{
	return askResolvedSignal;
}

void Ask::incrementTradedCountBy(const int inAmount)
{
	tradedCount += inAmount;
}

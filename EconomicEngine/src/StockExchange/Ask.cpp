#include "StockExchange/Ask.h"

Ask::Ask(const size_t id, const int count, const float price) : count(count), price(price), status(AskStatus::Pending), typeId(id), tradedCount(0) {}

void Ask::resolve()
{
	askResolvedSignal(this);
}

float Ask::getPrice() const
{
	return price;
}

int Ask::getCount() const
{
	return count;
}

size_t Ask::getId() const
{
	return typeId;
}

AskStatus Ask::getStatus() const
{
	return status;
}

void Ask::setPrice(const float newPrice)
{
	price = newPrice;
}

void Ask::setStatus(const AskStatus newStatus)
{
	status = newStatus;
}

int Ask::getTradedCount() const
{
	return tradedCount;
}

const Signal<Ask*>& Ask::getAskResolvedSignal() const
{
	return askResolvedSignal;
}

void Ask::incrementTradedCountBy(const int newCount)
{
	tradedCount += newCount;
}


BuyingAsk::BuyingAsk(const size_t id, const int newCount, const float newPrice) : Ask(id, newCount, newPrice){}

SellingAsk::SellingAsk(const size_t id, const int newCount, const float newPrice) : Ask(id, newCount, newPrice){}

#include "StockExchange/Ask.h"

#include "StockExchange/StockExchange.h"

Ask::Ask(const size_t id, const int count, const float price) : count(count), price(price), date(StockExchange::getInstance()->getTurnCount()), status(AskStatus::Pending), typeId(id), tradedCount(0) {}

float Ask::getPrice() const
{
	return price;
}

int Ask::getCount() const
{
	return count;
}

int Ask::getDate() const
{
	return date;
}

size_t Ask::getId() const
{
	return typeId;
}

AskStatus Ask::getStatus() const
{
	return status;
}

void Ask::setPrice(const float price)
{
	this->price = price;
}

void Ask::setStatus(const AskStatus status)
{
	this->status = status;
}

int Ask::getTradedCount() const
{
	return tradedCount;
}

void Ask::incrementTradedCountBy(const int count)
{
	tradedCount += count;
}


BuyingAsk::BuyingAsk(const size_t id, const int count, const float price) : Ask(id, count, price){}

SellingAsk::SellingAsk(const size_t id, const int count, const float price) : Ask(id, count, price){}

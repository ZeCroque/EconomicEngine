#include "Ask.h"
#include "Money.h"
#include "TurnManager.h"


Ask::Ask(const size_t id, const float count, const float price) : count(count), price(price), date(TurnManager::getInstance()->getTurnNumber()), status(AskStatus::Pending), typeId(id) {}

float  Ask::getPrice() const
{
	return price;
}

float Ask::getCount() const
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

BuyingAsk::BuyingAsk(const size_t id, const float count, const float price) : Ask(id, count, price){}

std::pair<size_t, float> BuyingAsk::getResult()
{
	return std::pair<size_t, float>(typeId, count);
}

SellingAsk::SellingAsk(const size_t id, const float count, const float price) : Ask(id, count, price){}

std::pair<size_t, float> SellingAsk::getResult()
{
	return std::pair<size_t, float>(typeid(Money).hash_code(), price);
}

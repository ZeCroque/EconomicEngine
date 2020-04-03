#include "Ask.h"

#include <typeinfo>

#include "Countable.h"
#include "Money.h"


Ask::Ask()
{
	status = AskStatus::Pending;
	date = 0; //TODO turnCount
	typeId = 0;
	price = 0;
}

Ask::Ask(Tradable& item, const int price) : Ask()
{
	content.emplace_back(item);
	typeId = typeid(item).hash_code();
	this->price = price;
}

Ask::Ask(std::vector<std::reference_wrapper<Tradable>> items, const int price) : Ask()
{
	content = std::move(items);
	if (!content.empty())
	{
		typeId = typeid(content[0]).hash_code();
	}
	this->price = price;
}

int Ask::getPrice() const
{
	return price;
}

int Ask::getCount() const
{
	if (content.empty())
	{
		auto* countable = dynamic_cast<Countable*>(&content[0].get());
		if (countable != nullptr)
		{
			return countable->getCount();
		}
		else
		{
			return static_cast<int>(content.size());
		}
	}
	return 0;
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



BuyingAsk::BuyingAsk(Tradable& item, const int price) : Ask::Ask(item, price) {}

BuyingAsk::BuyingAsk(std::vector<std::reference_wrapper<Tradable>> items, const int price) : Ask::Ask(items, price) {}

std::vector<std::reference_wrapper<Tradable>> BuyingAsk::getContent()
{
	return content;
}



SellingAsk::SellingAsk(Tradable& item, const int price) : Ask::Ask(item, price) {}

SellingAsk::SellingAsk(std::vector<std::reference_wrapper<Tradable>> items, const int price) : Ask::Ask(items, price) {}

std::vector<std::reference_wrapper<Tradable>> SellingAsk::getContent()
{
	std::vector<std::reference_wrapper<Tradable>> result;
	Money m(static_cast<int>(price));
	result.emplace_back(m);
	return result;
}
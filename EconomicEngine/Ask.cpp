#include "Ask.h"


#include <cassert>
#include <typeinfo>

#include "Countable.h"
#include "Money.h"
#include "Uncountable.h"


Ask::Ask()
{
	status = AskStatus::Pending;
	date = 0; //TODO turnCount
	typeId = 0;
	price = 0;
}

Ask::Ask(Countable& item, const int price) : Ask()
{
	content.emplace_back(item);
	typeId = typeid(item).hash_code();
	this->price = price;
}

Ask::Ask(std::vector<std::reference_wrapper<Tradable>> items, const int price) : Ask()
{
	assert(!items.empty());
	for(auto item : items)
	{
		assert(dynamic_cast<Uncountable*>(&item.get()) != nullptr);
	}
	

	content = std::move(items);
	typeId = typeid(content[0]).hash_code();
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

std::vector<std::reference_wrapper<Tradable>> Ask::getContent() const
{
	return content;
}


BuyingAsk::BuyingAsk(Countable& item, const int price) : Ask::Ask(item, price) {}

BuyingAsk::BuyingAsk(std::vector<std::reference_wrapper<Tradable>> items, const int price) : Ask::Ask(items, price) {}

std::vector<std::reference_wrapper<Tradable>> BuyingAsk::getResult()
{
	return content;
}



SellingAsk::SellingAsk(Countable& item, const int price) : Ask::Ask(item, price) {}

SellingAsk::SellingAsk(std::vector<std::reference_wrapper<Tradable>> items, const int price) : Ask::Ask(items, price) {}

std::vector<std::reference_wrapper<Tradable>> SellingAsk::getResult()
{
	std::vector<std::reference_wrapper<Tradable>> result;
	Money m(static_cast<int>(price));
	result.emplace_back(m);
	return result;
}
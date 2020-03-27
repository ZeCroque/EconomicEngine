#ifndef ASK_H
#define ASK_H
#include <type_traits>

#include "Tradable.h"

template<class T> class Ask
{
	enum class AskStatus {Pending, Sold, Refused};
	
private:
	float price;
	int count;
	int date;
	AskStatus status;
	T** result;
	
public:
	Ask() : price(0.0f), count(0), date(0), status(AskStatus::Pending), result(nullptr)
	{
		static_assert(std::is_base_of<Tradable, T>::value);
	}
	
	Ask(const float price, const int count, const int date) : price(price), count(count), date(date), status(AskStatus::Pending), result(nullptr)
	{
		static_assert(std::is_base_of<Tradable, T>::value);
	}
	
	float getPrice() const
	{
		return price;
	}

	int getCount() const
	{
		return count;
	}

	int getDate() const
	{
		return date;
	}

	AskStatus getStatus() const
	{
		return status;
	}

	T** getResult() const
	{
		if (status == AskStatus::Sold)
		{
			//TODO countable/uncountable
			return result;
		}
		return nullptr;
	}
};

template<class T> class BuyingAsk : public Ask<T> {};
template<class T> class SellingAsk : public Ask<T> {};

#endif

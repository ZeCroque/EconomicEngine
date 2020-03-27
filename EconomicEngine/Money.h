#ifndef MONEY_H
#define MONEY_H
#include <type_traits>

#include "Countable.h"
#include "Currency.h"

template<class T> class Money : public Countable
{
private:
	T* currency;
public:
	Money() : Countable()
	{
		static_assert(std::is_base_of<Currency, T>::value);
		currency = T::getInstance();
	}
	Money(int count) : Countable(count)
	{
		static_assert(std::is_base_of<Currency, T>::value);
		currency = T::getInstance();
	}
};

#endif
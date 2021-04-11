#ifndef ASK_FACTORY_H
#define ASK_FACTORY_H
#include "Ask.h"

template<class T> class AskFactory
{
public:
	AskFactory() = default;
	virtual ~AskFactory() = 0;
	
	SellingAsk<T>* createSellingAsk(const float price, const int count, const int date)
	{
		return new SellingAsk<T>(price, count, date);
	}

	BuyingAsk<T>* createBuyingAsk(const float price, const int count, const int date)
	{
		return new BuyingAsk<T>(price, count, date);
	}
};

#endif
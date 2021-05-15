#ifndef ASK_H
#define ASK_H

#include "Signal.h"

enum class AskStatus { Pending, Sold, Refused };

class Ask
{
protected:
	
	int count;
	float price;
	bool bIsSellingAsk;
	AskStatus status;
	size_t id;
	int tradedCount;

	Signal<Ask*> askResolvedSignal;
	
public:	
	Ask() = delete;
	Ask(bool bInIsSellingAsk, size_t inId, int inCount, float inPrice);
	void resolve();

	[[nodiscard]] bool isSellingAsk() const;
	[[nodiscard]] float getPrice() const;
	[[nodiscard]] int getCount() const;
	[[nodiscard]] size_t getId() const;
	[[nodiscard]] AskStatus getStatus() const;
	[[nodiscard]] int getTradedCount() const;
	[[nodiscard]] const Signal<Ask*>& getAskResolvedSignal() const;
	void setPrice(float inPrice);
	void setStatus(AskStatus inStatus);
	void incrementTradedCountBy(int inCount);

};

#endif //ASK_H

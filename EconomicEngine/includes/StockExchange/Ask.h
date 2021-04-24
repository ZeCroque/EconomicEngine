#ifndef ASK_H
#define ASK_H
#include <Signal.h>

enum class AskStatus { Pending, Sold, Refused };

class Ask
{
protected:
	
	int count;
	float price;
	bool isSellingAsk;
	AskStatus status;
	size_t typeId;
	int tradedCount;

	Signal<Ask*> askResolvedSignal;
	
public:	
	Ask() = delete;
	Ask(bool inIsSellingAsk, size_t id, int count, float price);
	void resolve();

	[[nodiscard]] bool getIsSellingAsk() const;
	[[nodiscard]] float getPrice() const;
	[[nodiscard]] int getCount() const;
	[[nodiscard]] size_t getId() const;
	[[nodiscard]] AskStatus getStatus() const;
	[[nodiscard]] int getTradedCount() const;
	[[nodiscard]] const Signal<Ask*>& getAskResolvedSignal() const;
	void setPrice(float newPrice);
	void setStatus(AskStatus newStatus);
	void incrementTradedCountBy(int newCount);

};

#endif

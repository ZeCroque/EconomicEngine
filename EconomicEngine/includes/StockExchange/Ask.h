#ifndef ASK_H
#define ASK_H
#include <Signal.h>

enum class AskStatus { Pending, Sold, Refused };

class Ask
{
protected:
	
	int count;
	float price;
	AskStatus status;
	size_t typeId;
	int tradedCount;

	Signal<Ask*> askResolvedSignal;
	
public:	
	Ask() = delete;
	Ask(size_t id, int count, float price);
	void resolve();
	
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

class BuyingAsk final : public Ask
{
public:
	BuyingAsk(size_t id, int newCount, float newPrice);
};
class SellingAsk final : public Ask
{
public:
	SellingAsk(size_t id, int newCount, float newPrice);
};

#endif

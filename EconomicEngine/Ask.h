#ifndef ASK_H
#define ASK_H

#include "Tradable.h"

enum class AskStatus { Pending, Sold, Refused };

class Ask
{
protected:
	
	float count;
	float price;
	int date;
	AskStatus status;
	size_t typeId;
	
public:	
	Ask() = delete;
	virtual ~Ask() = default;
	Ask(size_t id, float count, float price);
	Ask(Ask& a) = default;
	Ask(Ask&& a) = default;
	Ask& operator=(const Ask& a) = default;
	Ask& operator=(Ask&& a) = default;
	
	[[nodiscard]] float getPrice() const;
	[[nodiscard]] float getCount() const;
	[[nodiscard]] int getDate() const;
	[[nodiscard]] size_t getId() const;
	[[nodiscard]] AskStatus getStatus() const;
	void setPrice(float price);
	void setStatus(AskStatus status);
	virtual std::pair<size_t, float> getResult() = 0;

};

class BuyingAsk final : public Ask
{
public:
	BuyingAsk(size_t id, float count, float price);
	std::pair<size_t, float> getResult() override;
};
class SellingAsk final : public Ask
{
public:
	SellingAsk(size_t id, float count, float price);
	std::pair<size_t, float> getResult() override;
};

#endif

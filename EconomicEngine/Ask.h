#ifndef ASK_H
#define ASK_H

enum class AskStatus { Pending, Sold, Refused };

class Ask
{
protected:
	
	int count;
	float price;
	int date;
	AskStatus status;
	size_t typeId;
	int tradedCount;
	
public:	
	Ask() = delete;
	virtual ~Ask() = default;
	Ask(size_t id, int count, float price);
	Ask(Ask& a) = default;
	Ask(Ask&& a) = default;
	Ask& operator=(const Ask& a) = default;
	Ask& operator=(Ask&& a) = default;
	
	[[nodiscard]] float getPrice() const;
	[[nodiscard]] int getCount() const;
	[[nodiscard]] int getDate() const;
	[[nodiscard]] size_t getId() const;
	[[nodiscard]] AskStatus getStatus() const;
	[[nodiscard]] int getTradedCount() const;
	void setPrice(float price);
	void setStatus(AskStatus status);
	void incrementTradedCountBy(int count);

};

class BuyingAsk final : public Ask
{
public:
	BuyingAsk(size_t id, int count, float price);
};
class SellingAsk final : public Ask
{
public:
	SellingAsk(size_t id, int count, float price);
};

#endif

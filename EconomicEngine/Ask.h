#ifndef ASK_H
#define ASK_H

#include<map>

enum class AskStatus { Pending, Sold, Refused };

class Ask
{
protected:
	
	int count;
	float price;
	int date;
	AskStatus status;
	size_t typeId;
	
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
	void setPrice(float price);
	void setStatus(AskStatus status);

};

class BuyingAsk final : public Ask
{
public:
	BuyingAsk(size_t id, int count, float price);
};
class SellingAsk final : public Ask
{
private:
	int soldCount;
public:
	SellingAsk(size_t id, int count, float price);
	void incrementSoldCountBy(int count);
};

#endif

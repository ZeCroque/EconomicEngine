#ifndef ASK_H
#define ASK_H

#include <vector>
#include "Tradable.h"
#include "Countable.h"

enum class AskStatus { Pending, Sold, Refused };

class Ask
{
protected:
	
	std::vector<std::reference_wrapper<Tradable>> content;
	int price;
	int date;
	AskStatus status;
	size_t typeId;
	
public:	
	Ask();
	virtual ~Ask() = default;
	Ask(Countable& item, const int price);
	Ask(std::vector<std::reference_wrapper<Tradable>> items, const int price);
	Ask(Ask& a) = default;
	Ask(Ask&& a) = default;
	Ask& operator=(const Ask& a) = default;
	Ask& operator=(Ask&& a) = default;
	
	[[nodiscard]] int getPrice() const;
	[[nodiscard]] int getCount() const;
	[[nodiscard]] int getDate() const;
	[[nodiscard]] size_t getId() const;
	[[nodiscard]] AskStatus getStatus() const;
	[[nodiscard]] std::vector<std::reference_wrapper<Tradable>> getContent() const;
	virtual std::vector<std::reference_wrapper<Tradable>> getResult() = 0;

};

class BuyingAsk final : public Ask
{
public:
	BuyingAsk() = delete;
	BuyingAsk(Countable& item, const int price);
	BuyingAsk(std::vector<std::reference_wrapper<Tradable>> items, const int price);
	std::vector<std::reference_wrapper<Tradable>> getResult() override;
};
class SellingAsk final : public Ask
{
public:
	SellingAsk() = delete;
	SellingAsk(Countable& item, const int price);
	SellingAsk(std::vector<std::reference_wrapper<Tradable>> items, const int price);
	std::vector<std::reference_wrapper<Tradable>> getResult() override;
};

#endif

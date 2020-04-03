#ifndef ASK_H
#define ASK_H

#include <vector>
#include "Tradable.h"

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
	Ask(Tradable& item, const int price);
	Ask(Ask& a) = default;
	Ask(Ask&& a) = default;
	Ask& operator=(const Ask& a) = default;
	Ask& operator=(Ask&& a) = default;
	
	Ask(std::vector<std::reference_wrapper<Tradable>> items, const int price);

	[[nodiscard]] int getPrice() const;
	[[nodiscard]] int getCount() const;
	[[nodiscard]] int getDate() const;
	[[nodiscard]] size_t getId() const;
	[[nodiscard]] AskStatus getStatus() const;
	virtual std::vector<std::reference_wrapper<Tradable>> getContent() = 0;

};

class BuyingAsk final : public Ask
{
public:
	BuyingAsk() = delete;
	BuyingAsk(Tradable& item, const int price);
	BuyingAsk(std::vector<std::reference_wrapper<Tradable>> items, const int price);
	std::vector<std::reference_wrapper<Tradable>> getContent() override;
};
class SellingAsk final : public Ask
{
public:
	SellingAsk() = delete;
	SellingAsk(Tradable& item, const int price);
	SellingAsk(std::vector<std::reference_wrapper<Tradable>> items, const int price);
	std::vector<std::reference_wrapper<Tradable>> getContent() override;
};

#endif

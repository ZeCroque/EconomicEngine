#include "Traders/Craft.h"
#include "Traders/Trader.h"

Craft::Craft() : elapsedTime(0.0f), rate(0.0f), count(), result(0){}

Craft::Craft(const float inBaseRate, const size_t inCraftResult, const int inCount, std::list<std::pair<size_t, int>> inRequirements, std::list<size_t> inToolsRequired) : elapsedTime(0.0f), rate(inBaseRate), count(inCount), result(inCraftResult), requirements(std::move(inRequirements)), toolsRequired(std::move(inToolsRequired)){}
	                                                                                                                           
Craft::Craft(Craft& craft) : elapsedTime(craft.elapsedTime), rate(craft.rate), count(craft.count), result(craft.result), requirements(craft.requirements), toolsRequired(craft.toolsRequired) {}

Craft* Craft::clone()
{
	return new Craft(*this);
}

std::list<std::pair<size_t, int>> Craft::getRequirement() const
{
	return requirements;
}

std::list<size_t> Craft::getToolsRequired() const
{
	return toolsRequired;
}

size_t Craft::getResult() const
{
	return result;
}

int Craft::getCount() const
{
	return count;
}

float Craft::getRate() const
{
	return rate;
}

const Signal<>& Craft::getCraftSuccessSignal() const
{
	return craftSuccessSignal;
}

void Craft::incrementRate(const float inAmount)
{
	rate += inAmount;
}

void Craft::update(const float inDeltaTime)
{
	elapsedTime += inDeltaTime;
	if(elapsedTime >= EconomicEngine::getInstance()->getBaseActionTime() * rate)
	{
		elapsedTime = 0.f;
		craftSuccessSignal();
	}
}

			
			
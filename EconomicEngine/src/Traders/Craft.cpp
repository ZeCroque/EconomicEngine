#include "Traders/Craft.h"

#include "Traders/Trader.h"

Craft::Craft() : elapsedTime(0.0f), rate(0.0f), count(), result(0){}

Craft::Craft(const float baseRate, const size_t craftResult, const int count, std::list<std::pair<size_t, int>> requirements, std::list<size_t> toolsRequired) : elapsedTime(0.0f), rate(baseRate), count(count), result(craftResult), requirements(std::move(requirements)), toolsRequired(std::move(toolsRequired)){}
	                                                                                                                           
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

void Craft::incrementRate(const float i)
{
	rate += i;
}

void Craft::update(float deltaTime)
{
	elapsedTime += deltaTime;
	if(elapsedTime >= EconomicEngine::getInstance()->getBaseActionTime() * rate)
	{
		elapsedTime = 0.f;
		craftSuccessSignal();
	}
}

			
			
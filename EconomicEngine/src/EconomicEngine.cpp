#include "EconomicEngine.h"

#include <filesystem>
#include <fstream>

#include "Traders/Trader.h"
#include "Tradables/Food.h"
#include "Tradables/Uncountable/ToolBehavior.h"
#include "Tradables/Uncountable/Uncountable.h"

void EconomicEngine::initJobs(std::vector<nlohmann::json>& parsedJobs) const
{
	const std::hash<std::string> hasher;
	for(const auto& parsedJob : parsedJobs)
	{
		auto* job = new Job(parsedJob["name"]);
		
		for(const auto& parsedCraft : parsedJob["crafts"])
		{	
			std::list<std::pair<size_t, int>> requirements;
			for(const auto& requirement : parsedCraft["requirements"])
			{
				requirements.emplace_back(std::pair<size_t, int>(hasher(requirement["name"]), requirement["count"]));
			}

			std::list<size_t> requiredTools;
			for(const auto& requiredTool : parsedCraft["requiredTools"])
			{
				requiredTools.emplace_back(hasher(static_cast<std::string>(requiredTool) + "Behavior"));
			}
			
			 job->getCraftFactory()->registerCraft(new Craft(parsedCraft["baseRate"], hasher(parsedCraft["result"]), parsedCraft["producedCount"], requirements, requiredTools));
		}

		for(const auto& parsedTool : parsedJob["usableTools"])
		{
			job->getUsableTools().emplace_back(hasher(parsedTool));
		}

		traderManager.registerJob(job);
	}
}

void EconomicEngine::initTradables(std::vector<nlohmann::json>& parsedTradables) const
{
	for(const auto& parsedTradable : parsedTradables)
	{
		Tradable* tradable = nullptr;
		const std::pair<float, float> defaultPriceBelief(parsedTradable["defaultPriceBelief"]["min"], parsedTradable["defaultPriceBelief"]["max"]);
		
		if(parsedTradable["type"] == "Countable")
		{
			tradable = new Countable(parsedTradable["name"], defaultPriceBelief);
		}
		else if(parsedTradable["type"] == "Uncountable")
		{
			tradable = new Uncountable(parsedTradable["name"], defaultPriceBelief, new ToolBehavior(parsedTradable["behavior"]["craftRateBoost"], parsedTradable["behavior"]["degradationRate"]));
		}
		else if(parsedTradable["type"] == "Food")
		{
			tradable = new Food(parsedTradable["name"], defaultPriceBelief, parsedTradable["foodValue"]);
		}
		const std::hash<std::string> hasher;
		tradableFactory.registerObject(hasher(parsedTradable["name"]),tradable);
	}	
}

void EconomicEngine::init(const char* prefabsPath) const
{
	assert(std::filesystem::exists(prefabsPath) && std::filesystem::is_directory(prefabsPath));

	std::vector<nlohmann::json> jobs;
	std::vector<nlohmann::json> tradables;
	
	std::ifstream fileStream;
	for (const auto& entry : std::filesystem::recursive_directory_iterator(prefabsPath))
	{
		if (std::filesystem::is_regular_file(entry.status()) && entry.path().extension() == ".json")
		{
			nlohmann::json parsedJson;
			
			fileStream.open (entry.path());
			fileStream >> parsedJson;
		    fileStream.close();

			if(parsedJson["type"] == "Job")
			{
				jobs.push_back(parsedJson);
			}
			else if(parsedJson["type"] == "Countable" || parsedJson["type"] == "Uncountable" || parsedJson["type"] == "Food")
			{
				tradables.push_back(parsedJson);
			}
		}
	}
	initJobs(jobs);
	initTradables(tradables);
	
	traderManager.init();
	stockExchange.init();
}

void EconomicEngine::start(const int count)
{
	bRunning = true;

	//Create traders
	traderManager.addTrader(count);
	
}

void EconomicEngine::update(float deltaTime)
{
	if(bRunning)
	{
		elapsedTimeSinceDayStart += deltaTime;
		if(elapsedTimeSinceDayStart >= dayDuration)
		{
			elapsedTimeSinceDayStart = 0.f;
			++elapsedDayCount;
		}
		elapsedTimeSinceLastStockExchangeResolution += deltaTime;
		if(elapsedTimeSinceLastStockExchangeResolution >= stockExchangeResolutionTime)
		{
			elapsedTimeSinceLastStockExchangeResolution = 0.f;
			stockExchange.resolveOffers();
			traderManager.killStarvedTraders();
		}
		traderManager.update(deltaTime);
	}
}

void EconomicEngine::reset(const int count)
{
	elapsedTimeSinceDayStart = 0.f;
	elapsedTimeSinceLastStockExchangeResolution = 0.f;
	elapsedDayCount = 0;
	traderManager.reset();
	stockExchange.reset();
	traderManager.addTrader(count);
}

void EconomicEngine::pause()
{
	bRunning = false;
}

void EconomicEngine::resume()
{
	bRunning = true;
}

float EconomicEngine::getBaseActionTime() const
{
	return baseActionTime;
}

int EconomicEngine::getElapsedDayCount() const
{
	return elapsedDayCount;
}

TradableFactory& EconomicEngine::getTradableFactory() const
{
	return tradableFactory;
}

TraderManager& EconomicEngine::getTraderManager() const
{
	return traderManager;
}

StockExchange& EconomicEngine::getStockExchange() const
{
	return stockExchange;
}

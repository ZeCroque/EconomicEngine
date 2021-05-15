#include <filesystem>
#include <fstream>

#include "EconomicEngine.h"
#include "Traders/Trader.h"
#include "Tradables/Food.h"
#include "Tradables/Uncountable/ToolBehavior.h"
#include "Tradables/Uncountable/Uncountable.h"

void EconomicEngine::init(const char* prefabsPath) const
{
	assert(std::filesystem::exists(prefabsPath) && std::filesystem::is_directory(prefabsPath));

	std::vector<nlohmann::json> jobs;
	std::vector<nlohmann::json> tradables;

	for (std::ifstream fileStream; const auto& entry : std::filesystem::recursive_directory_iterator(prefabsPath))
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

void EconomicEngine::start(const int inTradersAmount)
{
	bRunning = true;

	//Create traders
	traderManager.addTraders(inTradersAmount);
	
}

void EconomicEngine::pause()
{
	bRunning = false;
}

void EconomicEngine::resume()
{
	bRunning = true;
}

void EconomicEngine::reset(const int inTradersAmount)
{
	elapsedTimeSinceDayStart = 0.f;
	elapsedTimeSinceLastStockExchangeResolution = 0.f;
	elapsedDayCount = 0;
	traderManager.reset();
	stockExchange.reset();
	traderManager.addTraders(inTradersAmount);
}

void EconomicEngine::update(const float inDeltaTime)
{
	if(bRunning)
	{
		elapsedTimeSinceDayStart += inDeltaTime;
		if(elapsedTimeSinceDayStart >= dayDuration)
		{
			elapsedTimeSinceDayStart = 0.f;
			traderManager.makeChildren();
			traderManager.killStarvedTraders();
			++elapsedDayCount;
		}
		elapsedTimeSinceLastStockExchangeResolution += inDeltaTime;
		if(elapsedTimeSinceLastStockExchangeResolution >= stockExchangeResolutionTime)
		{
			elapsedTimeSinceLastStockExchangeResolution = 0.f;
			stockExchange.resolveOffers();
			traderManager.clearPendingKillTraders();

		}
		traderManager.update(inDeltaTime);
	}
}

EconomicEngine::EconomicEngine() : bRunning(false), elapsedDayCount(0), elapsedTimeSinceDayStart(0), dayDuration(24.f),
                                   elapsedTimeSinceLastStockExchangeResolution(0.f),
                                   stockExchangeResolutionTime(dayDuration / 12.f), baseActionTime(dayDuration / 6.f)
{
	
}

void EconomicEngine::initJobs(std::vector<nlohmann::json>& inParsedJobs) const
{
	for(const std::hash<std::string> hasher; const auto& parsedJob : inParsedJobs)
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
			for(const auto& requiredTool : parsedCraft["requiredToolBehaviors"])
			{
				requiredTools.emplace_back(hasher(requiredTool));
			}
			
			 job->getCraftFactory()->registerCraft(new Craft(parsedCraft["baseRate"], hasher(parsedCraft["result"]), parsedCraft["producedCount"], requirements, requiredTools));
		}

		for(const auto& parsedTool : parsedJob["usableTools"])
		{
			job->addUsableTool(hasher(parsedTool));
		}

		traderManager.registerJob(job);
	}
}

void EconomicEngine::initTradables(std::vector<nlohmann::json>& inParsedTradables) const
{
	for(const auto& parsedTradable : inParsedTradables)
	{
		Tradable* tradable = nullptr;

		if(const std::pair<float, float> defaultPriceBelief(parsedTradable["defaultPriceBelief"]["min"], parsedTradable["defaultPriceBelief"]["max"]); parsedTradable["type"] == "Countable")
		{
			tradable = new Countable(parsedTradable["name"], defaultPriceBelief);
		}
		else if(parsedTradable["type"] == "Uncountable")
		{
			tradable = new Uncountable(parsedTradable["name"], defaultPriceBelief, new ToolBehavior(parsedTradable["behavior"]["name"],parsedTradable["behavior"]["craftRateBoost"], parsedTradable["behavior"]["degradationRate"]));
		}
		else if(parsedTradable["type"] == "Food")
		{
			tradable = new Food(parsedTradable["name"], defaultPriceBelief, parsedTradable["foodValue"]);
		}
		const std::hash<std::string> hasher;
		tradableFactory.registerObject(hasher(parsedTradable["name"]),tradable);
	}	
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
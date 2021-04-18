#include "EconomicEngine.h"

#include <filesystem>
#include <fstream>


#include "Tradables/Food.h"
#include "Tradables/Uncountable/ToolBehavior.h"
#include "Tradables/Uncountable/Uncountable.h"

const Signal<>& EconomicEngine::getAsksResolvedSignal() const
{
	return asksResolvedSignal;
}

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

		traderManager->registerJob(job);
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
	
	traderManager->init();
	stockExchange->init();
}

void EconomicEngine::start(const int count)
{
	bRunning = true;

	//Create traders
	traderManager->addTrader(count);
	
}

void EconomicEngine::update(float deltaTime)
{

	if(bRunning)
	{
		elapsedTimeSinceDayStart += deltaTime;
		if(elapsedTimeSinceDayStart >= dayDuration)
		{
			elapsedTimeSinceDayStart = 0.f;
			traderManager->killStarvedTraders();
		}
		
		traderManager->update(deltaTime);
		//TODO stockexchange
	}
}

void EconomicEngine::reset(const int count)
{
	elapsedTimeSinceDayStart = 0.f;
	traderManager->reset();
	stockExchange->reset();
	traderManager->addTrader(count);
}

/*int EconomicEngine::exec(const int count)
{
	this->bRunning = true;

	//Create traders
	traderManager->addTrader(count);

	while (bRunning)
	{
		while (bPaused)
		{
			std::unique_lock<std::mutex> lk(m);
			cv.wait(lk);
			lk.unlock();
		}
		for (auto i = 0; i < this->step; i++)
		{
			stockExchange->incrementTurnCount();
			
			traderManager->doTradersCrafting();
			traderManager->doTradersAsking();
			stockExchange->resolveOffers();
			asksResolvedSignal();
			
			traderManager->refreshTraders();
			traderManager->killTraders();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / turnSecond));
	}
	return 0;
}*/

/*void EconomicEngine::stop()
{
	asksResolvedSignal.disconnectAll();
	bRunning = false;
}*/

void EconomicEngine::pause()
{
	bRunning = false;
}

void EconomicEngine::resume()
{
	bRunning = false;
}

void EconomicEngine::setTurnSecond(const int turnSecond)
{
	this->turnSecond = turnSecond;
}

int EconomicEngine::getTurnCount() const
{
	return stockExchange->getTurnCount();
}

void EconomicEngine::setStep(const int step)
{
	this->step = step;
}

float EconomicEngine::getBaseActionTime() const
{
	return baseActionTime;
}

const TradableFactory& EconomicEngine::getTradableFactory() const
{
	return tradableFactory;
}

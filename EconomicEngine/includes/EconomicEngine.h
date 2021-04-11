#ifndef TURN_MANAGER_H
#define TURN_MANAGER_H
#include <filesystem>

#include "Observable.h"
#include "Singleton.h"
#include "StockExchange.h"
#include "TradableManager.h"
#include "TraderManager.h"
#include "nlohmann/json.hpp"
#include <mutex>
#include <thread>
#include <fstream>

#include "Food.h"

template <class T> class EconomicEngine : public Observable, public Singleton<T>
{
protected:
	TraderManager* traderManager;
	TradableManager* tradableManager;
	StockExchange* stockExchange;

	EconomicEngine() : traderManager(TraderManager::getInstance()), tradableManager(TradableManager::getInstance()), stockExchange(StockExchange::getInstance()), bRunning(false), bPaused(true), turnSecond(1), step(1) {}
	virtual ~EconomicEngine() = default;
private:
	bool bRunning;
	bool bPaused;
	int turnSecond;
	int step;
	std::condition_variable cv;
	std::mutex m;

public:

	void initJobs(std::vector<nlohmann::json>& parsedJobs) const
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

			for(const auto parsedTool : parsedJob["usableTools"])
			{
				job->getUsableTools().emplace_back(hasher(parsedTool));
			}

			traderManager->registerJob(job);
		}
	}
	
	void initTradables(std::vector<nlohmann::json>& parsedTradables) const
	{
		for(const auto& parsedTradable : parsedTradables)
		{
			Tradable* tradable = nullptr;
			std::pair<float, float> defaultPriceBelief(parsedTradable["defaultPriceBelief"]["min"], parsedTradable["defaultPriceBelief"]["max"]);
			
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

			tradableManager->registerTradable(tradable);
		}	
	}
	
	void init(const char* prefabsPath) const
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

	void reset(const int count) const
	{
		traderManager->reset();
		stockExchange->reset();
		traderManager->addTrader(count);
	}

	int exec(const int count)
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
				traderManager->refreshTraders();
				traderManager->killTraders();
			}

			this->notifyObservers();
			std::this_thread::sleep_for(std::chrono::milliseconds(1000 / turnSecond));
		}
		return 0;
	}

	void stop()
	{
		if(bPaused)
		{
			resume();
		}
		this->bRunning = false;
	}

	void pause()
	{
		std::lock_guard<std::mutex> lk(m);
		this->bPaused = true;
	}

	void resume()
	{
		std::lock_guard<std::mutex> lk(m);
		bPaused = false;
		cv.notify_one();
	}

	void setTurnSecond(const int turnSecond)
	{
		this->turnSecond = turnSecond;
	}

	[[nodiscard]] int getTurnCount() const
	{
		return stockExchange->getTurnCount();
	}

	void setStep(const int step)
	{
		this->step = step;
	}

};


#endif

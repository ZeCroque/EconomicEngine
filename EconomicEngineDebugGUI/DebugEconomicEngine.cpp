#include "DebugEconomicEngine.h"

#include "Axe.h"
#include "Baker.h"
#include "Bow.h"
#include "Bread.h"
#include "Butcher.h"
#include "Coal.h"
#include "Farmer.h"
#include "Hoe.h"
#include "Hunter.h"
#include "Iron.h"
#include "Leather.h"
#include "Lumberjack.h"
#include "Meat.h"
#include "Miner.h"
#include "Pickaxe.h"
#include "Steak.h"
#include "Sword.h"
#include "ToolMaker.h"
#include "Wheat.h"
#include "Wood.h"
#include "WoodPickaxe.h"

DebugEconomicEngine::DebugEconomicEngine() : EconomicEngine<DebugEconomicEngine>(){}

void DebugEconomicEngine::initTraders() const
{
	//Init jobs
	traderManager->registerJob(new Farmer());
	traderManager->registerJob(new Hunter());
	traderManager->registerJob(new Miner());
	traderManager->registerJob(new Baker());
	traderManager->registerJob(new Butcher());
	traderManager->registerJob(new ToolMaker());
	traderManager->registerJob(new Lumberjack());	
}

void DebugEconomicEngine::initTradables() const
{
	//Init tradables
	tradableManager->registerTradable(new Wheat());
	tradableManager->registerTradable(new Meat());
	tradableManager->registerTradable(new Wood());
	tradableManager->registerTradable(new Coal());
	tradableManager->registerTradable(new Bread());
	tradableManager->registerTradable(new Steak());
	tradableManager->registerTradable(new Iron());
	tradableManager->registerTradable(new Leather());

	//Init tools
	tradableManager->registerTradable(new WoodPickaxe());
	tradableManager->registerTradable(new Hoe());
	tradableManager->registerTradable(new Bow());
	tradableManager->registerTradable(new Axe());
	tradableManager->registerTradable(new Pickaxe());
	tradableManager->registerTradable(new Sword());
}
